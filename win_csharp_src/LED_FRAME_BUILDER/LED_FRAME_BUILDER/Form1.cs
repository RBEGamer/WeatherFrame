using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
namespace LED_FRAME_BUILDER
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            build_color_table();
        }


        List<Color> colors = new List<Color>();
        Color current_chosen_color = new Color();


        struct layer
        {
           public  int[,] matrix_cid;
           public string layer_name;
        }

        List<layer> layers = new List<layer>();

        int matrix_size_w = 0;
        int matrix_size_h = 0;
        int current_chosen_color_id = 0;
        int current_selected_layer = 0;
        const int clear_color_id = 0;
        int last_selected_layer = 0;
        private void build_color_table()
        {
            //TODO load from color table file ->input excel string

            colors.Clear();

            
            StreamReader sr= File.OpenText("nes_color_palette.csv");

            int count = 0;
            while (true)
            {
            
                string read_string = sr.ReadLine();
                if(read_string == null)
                {
                    break;
                }
                string[] sp_read = read_string.Split(';');
                Color tmc;
                try
                { 
                    int tmp = int.Parse(sp_read[2]);
                }
                catch (Exception)
                {

                    continue;
                }
                tmc = Color.FromArgb(int.Parse(sp_read[2]), int.Parse(sp_read[3]), int.Parse(sp_read[4]));
                
                colors.Add(tmc);
                count++;
                if (sr.EndOfStream) { break; }
            }
            sr.Close();



            int color_pw = color_chooser.Size.Width / 32;
            int color_ph = colors.Count / color_pw;
            color_chooser.Controls.Clear();
            for (int i = 0; i < colors.Count; i++)
            {
                PictureBox pic = new PictureBox();
                pic.Name = "colorpick_" + i.ToString();
                pic.Size = new Size(32, 32);
                pic.BackColor = colors[i];
                pic.Location = new Point((i % color_pw)*32,(i / color_ph)*32);
                pic.Click += click_color;

                color_chooser.Controls.Add(pic);
            }
        }


        private void set_matrix_size_btn_Click(object sender, EventArgs e)
        {
            set_matrix_size_btn.Enabled = false;
            matrix_panel.Controls.Clear();
          //  matrix_color_ids = new int[(int)matrix_size_widht.Value,(int)matrix_size_height.Value];
            int mw_pixel = matrix_panel.Size.Width / (int)matrix_size_widht.Value;
            int mh_pixel = matrix_panel.Size.Height / (int)matrix_size_height.Value;
            matrix_size_w = (int)matrix_size_widht.Value;
            matrix_size_h = (int)matrix_size_height.Value;

            //CREATE LAYER
            layer first_layer = new layer();
            first_layer.layer_name = "<defult_name>";
            first_layer.matrix_cid = new int[(int)matrix_size_widht.Value, (int)matrix_size_height.Value];

            for (int i = 0; i < (int)matrix_size_widht.Value; i++)
            {
                for (int j = 0; j < (int)matrix_size_height.Value; j++)
                {
                    PictureBox tmp = new PictureBox();
                    tmp.Size = new Size(mw_pixel, mh_pixel);
                    tmp.Location = new Point(i * mw_pixel, j* mh_pixel);
                    tmp.Name = "mcell_" + i.ToString() + "_" + j.ToString();
                    tmp.BackColor = Color.Black;
                    matrix_panel.Controls.Add(tmp);
                    tmp.Click += click_cell;
                    
                    first_layer.matrix_cid[i, j] = clear_color_id;
                }
            }
            first_layer.layer_name =  "layer_" + layers.Count().ToString();
            layers.Add(first_layer);
            layers_listbox.Items.Add(first_layer.layer_name);
            layers_listbox.SelectedIndex = 0;
        }
        private void click_cell(object sender, EventArgs e) //EVENT
        {
            PictureBox p = (PictureBox)sender;
            if (p != null)
            {
                p.BackColor = current_chosen_color;
                int pos_w = int.Parse(p.Name.Split('_')[1]);
                int pos_h = int.Parse(p.Name.Split('_')[2]);
                layers[current_selected_layer].matrix_cid[pos_w, pos_h] = current_chosen_color_id;
            }
        }
        private void click_color(object sender, EventArgs e) //EVENT
        {
            PictureBox p = (PictureBox)sender;
            if (p != null)
            {
                current_chosen_color = p.BackColor;
                current_chosen_color_id = int.Parse(p.Name.Split('_')[1]); // LOL
            }
        }
        private void button1_Click(object sender, EventArgs e)
        {
           // if(matrix_color_ids.Length() > 0){
                for (int i = 0; i < (int)matrix_size_widht.Value; i++)
                {
                    for (int j = 0; j < (int)matrix_size_height.Value; j++)
                    {
                         layers[current_selected_layer].matrix_cid[i, j] = clear_color_id;
                    }
                }
            //}
            //CLEAR ALL PICURE BOXES
            for (int i = 0; i < matrix_panel.Controls.Count; i++)
            {
                //TODO ADD TYPECHECK
                  PictureBox p = (PictureBox)matrix_panel.Controls[i];
                if(p != null)
                {
                    p.BackColor = colors[clear_color_id];
                }
            }
        }

        //ADD FRAME BTN
        private void button2_Click(object sender, EventArgs e)
        {
            last_selected_layer = current_selected_layer;
            layer tmp_layer = new layer();
            tmp_layer.layer_name = "layer_" + layers.Count.ToString();
            tmp_layer.matrix_cid = new int[(int)matrix_size_widht.Value, (int)matrix_size_height.Value];
            layers.Add(tmp_layer);
            layers_listbox.Items.Add(tmp_layer.layer_name);
            layers_listbox.SelectedItem = tmp_layer.layer_name;

           
            current_selected_layer = layers.Count-1;
            if(copy_frame_data_chbx.Checked)
            {
                for (int i = 0; i < (int)matrix_size_widht.Value; i++)
                {
                    for (int j = 0; j < (int)matrix_size_height.Value; j++)
                    {
                        layers[current_selected_layer].matrix_cid[i, j] = layers[last_selected_layer].matrix_cid[i, j];
                    }
                }
            }
        }

        //REMOVE FRAME BTN
        private void button3_Click(object sender, EventArgs e)
        {


        }


        //EXPORT

        private void eXPORTToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void layers_listbox_SelectedIndexChanged(object sender, EventArgs e)
        {
            ListBox llistbox = (ListBox)sender;
            last_selected_layer = current_selected_layer;

            current_selected_layer = llistbox.SelectedIndex;
            //TODO REPLACE WITH REF IN STRUCT
            for (int i = 0; i < (int)matrix_size_widht.Value; i++)
            {
                for (int j = 0; j < (int)matrix_size_height.Value; j++)
                {
                    for (int k = 0; k < matrix_panel.Controls.Count; k++)
                    {
                        if (matrix_panel.Controls[k].Name == "mcell_" + i.ToString() + "_" + j.ToString())
                        {
                            matrix_panel.Controls[k].BackColor = colors[layers[current_selected_layer].matrix_cid[i, j]];
                        }
                    }
                }
            }
            
        }
    }
}
