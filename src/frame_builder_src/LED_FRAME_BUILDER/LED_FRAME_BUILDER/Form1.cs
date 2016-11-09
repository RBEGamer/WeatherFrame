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

            try
            {
                //TODO REMOVE THE TRY SHIT
                StreamReader sra = File.OpenText("nes_color_palette.csv");
                sra.Close();
            }
            catch (Exception)
            {
                FileStream sw = File.Create("nes_color_palette.csv");
               //TODO ADD SAMPLE COLORS SIME R G B WHITE BLACK
                sw.Close();
            }

            StreamReader sr = File.OpenText("nes_color_palette.csv");


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


        private void export_layers()
        {
            /*
             * 
             * */
            string matrix_data = "";


            for (int i = 0; i < layers.Count; i++)
            {

                matrix_data += "FRAME_" + i.ToString() + "_"+ layers.Count.ToString() + "_" + matrix_size_w.ToString() + "_" + matrix_size_h.ToString() + "_" + "\n";


                for (int x = 0; x < matrix_size_w; x++)
                {
                    for (int y = 0; y < matrix_size_h; y++)
                    {
                        matrix_data += layers[i].matrix_cid[x, y].ToString() + ",";
                    }
                    matrix_data += "\n";
                }
                matrix_data += "\n";
            }

            saveFileDialog1.Filter = "FRAME_BUIDER_ANIMATION_FILE (.anim) | *.anim";
            if (saveFileDialog1.ShowDialog() == DialogResult.OK) { 
                System.IO.File.WriteAllText(saveFileDialog1.FileName, matrix_data);
            }else
            {
                MessageBox.Show("EXPORT FAILED");
            }
        }

        private void import_layers()
        {
            layers.Clear();
            layers_listbox.Items.Clear();



            openFileDialog1.FileName = "";
            openFileDialog1.Filter = "FRAME_BUIDER_ANIMATION_FILE (.anim) | *.anim";
            int frame_counter = 0;
            int frame_id = 0;


            int coll_counter = 0;
            layer tmp_layer;
            tmp_layer = new layer();
            tmp_layer.matrix_cid = new int[1, 1];


            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                StreamReader sr = File.OpenText(openFileDialog1.FileName);


                //first array
                string[] lines;
                string read_all = sr.ReadToEnd();
                lines = read_all.Split('\n');

                //Get frame size
                int frame_size_w = 0;
                int frame_size_h = 0;

                for (int i = 0; i < lines.Length; i++)
                {

                    if (lines[i].Contains("FRAME_"))
                    {
                        frame_counter++;
                        string[] split_content = lines[i].Split('_');
                        frame_id = int.Parse(split_content[1]);
                        int t_frame_size_w = int.Parse(split_content[3]);
                        int t_frame_size_h = int.Parse(split_content[4]);
                        if(t_frame_size_w > frame_size_w)
                        {
                            frame_size_w = t_frame_size_w;
                        }
                        if (t_frame_size_h > frame_size_h)
                        {
                            frame_size_h = t_frame_size_h;
                        }

                    }
                }
                   
              
                if(frame_size_h <= 0 || frame_size_w <= 0)
                {
                    MessageBox.Show("IMPORT FAILED -> FILE FRAME_SIZE INVALID");
                    return;
                }


                tmp_layer = new layer();
                tmp_layer.matrix_cid = new int[frame_size_w, frame_size_h];
                int frame_curr_counter = 0;

                for (int i = 0; i < lines.Length; i++)
                {
                    if (lines[i].Contains(','))
                    {
                        string[] split_col = lines[i].Split(',');
                        for (int j = 0; j < split_col.Length; j++)
                        {
                            if(split_col[j] == "")
                            {
                               coll_counter++;
                                break;
                            }
                            tmp_layer.matrix_cid[ coll_counter,j] = int.Parse(split_col[j]);
                        }
                    }
                    else if(lines[i] == "")
                    {
                        //reset col counter
                        coll_counter = 0;
                        tmp_layer.layer_name = "layer_" + frame_curr_counter.ToString();
                       
                        layers.Add(tmp_layer);
                        tmp_layer = new layer();
                        tmp_layer.matrix_cid = new int[frame_size_w, frame_size_h];
                        frame_curr_counter++;

                    }
                }


                for (int i = 0; i < layers.Count; i++)
                {
                    layers_listbox.Items.Add(layers[i].layer_name);
                }
                //ADD PICTURE BOX MATRIX
                matrix_panel.Controls.Clear();
                int mw_pixel = matrix_panel.Size.Width / (int)matrix_size_widht.Value;
                int mh_pixel = matrix_panel.Size.Height / (int)matrix_size_height.Value;
                matrix_size_w = (int)matrix_size_widht.Value;
                matrix_size_h = (int)matrix_size_height.Value;
                for (int i = 0; i < (int)matrix_size_widht.Value; i++)
                {
                    for (int j = 0; j < (int)matrix_size_height.Value; j++)
                    {
                        PictureBox tmp = new PictureBox();
                        tmp.Size = new Size(mw_pixel, mh_pixel);
                        tmp.Location = new Point(i * mw_pixel, j * mh_pixel);
                        tmp.Name = "mcell_" + i.ToString() + "_" + j.ToString();
                        tmp.BackColor = colors[ layers[0].matrix_cid[i, j]];
                        matrix_panel.Controls.Add(tmp);
                        tmp.Click += click_cell;
                    }
                }
                current_selected_layer = 0;
                last_selected_layer = 0;
                //CREATE NEW PICTUREBOX MATRIX
                //FILE ITEM BOX

            }
            else
            {
                MessageBox.Show("IMPORT FAILED OPEN FILE DIALOG ERROR");
                return;
            }
        }

        private void new_sheet()
        {
            set_matrix_size_btn_Click(null, null);
            neuToolStripMenuItem.Enabled = false;
        }

        private void set_matrix_size_btn_Click(object sender, EventArgs e)
        {
            //set_matrix_size_btn.Enabled = false;
            matrix_panel.Controls.Clear();
          //  matrix_color_ids = new int[(int)matrix_size_widht.Value,(int)matrix_size_height.Value];
            int mw_pixel = matrix_panel.Size.Width / (int)matrix_size_widht.Value;
            int mh_pixel = matrix_panel.Size.Height / (int)matrix_size_height.Value;
            matrix_size_w = (int)matrix_size_widht.Value;
            matrix_size_h = (int)matrix_size_height.Value;

            //CREATE LAYER
            layer first_layer = new layer();
            first_layer.layer_name = "<invalid_layer>";
            first_layer.matrix_cid = new int[(int)matrix_size_widht.Value, (int)matrix_size_height.Value];

            for (int i = 0; i < (int)matrix_size_widht.Value; i++)
            {
                for (int j = 0; j < (int)matrix_size_height.Value; j++)
                {
                    PictureBox tmp = new PictureBox();
                    tmp.Size = new Size(mw_pixel, mh_pixel);
                    tmp.Location = new Point(i * mw_pixel, j* mh_pixel);
                    tmp.Name = "mcell_" + i.ToString() + "_" + j.ToString();
                    tmp.BackColor = colors[clear_color_id];
                    matrix_panel.Controls.Add(tmp);
                    tmp.Click += click_cell;
                  //  first_layer.ref_box = tmp;
                    first_layer.matrix_cid[i, j] = clear_color_id;
                }
            }
            first_layer.layer_name =  "frame_" + layers.Count().ToString();
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
            tmp_layer.layer_name = "frame_" + layers.Count.ToString();
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

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void speichernToolStripMenuItem_Click(object sender, EventArgs e)
        {
            export_layers();
        }

        private void öffnenToolStripMenuItem_Click(object sender, EventArgs e)
        {
            import_layers();
        }

        private void neuToolStripMenuItem_Click(object sender, EventArgs e)
        {
            new_sheet();
        }

        private void speichernunterToolStripMenuItem_Click(object sender, EventArgs e)
        {
            export_layers();
        }
    }
}
