using System;
using System.IO;
using System.IO.Ports;
using System.Collections.Generic;
using System.Threading.Tasks;
using System.Linq;
using System.Text;
using System.Net.Http;
using System.Net.Http.Headers;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;


namespace TraductionAudioTexte
{
    class Program
    {

        private static FileStream fileStream;// = new FileStream(filename, FileMode.Create);
        private static MemoryStream raw_stream;    
        private static string message = "";
        static void Main(string[] args)
        {

            Menu();
            Console.WriteLine("Fermeture du programme !!");
        }
        private static void Menu()
        {
            string chaine = "";
            do
            {
                Console.WriteLine("Menu");
                Console.WriteLine(" - lecture sur le port série taper l");
                Console.WriteLine(" - traduction du fichier audio en texte taper tr");
                Console.WriteLine(" - transmission texte taper t");
                Console.WriteLine(" - finir taper fin");
                chaine = Console.ReadLine();

                if (chaine == "l")
                    LecturePort();

                if (chaine == "tr")
                 Traduction();

                if (chaine == "t")
                {
                    Transmission();
                }
            } while (chaine != "fin");
        }
        private static void LecturePort()
        {
            raw_stream = new MemoryStream();
            SerialPort p = new SerialPort(Config.SerialPortParameters.nomPort, Config.SerialPortParameters.vitesse, Parity.None, 
                                    Config.SerialPortParameters.bitDonnee, StopBits.One);
            p.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);
            p.Open();
            Console.WriteLine("Port série ouvert");
            Console.WriteLine("Lecture");
            Console.Read();
            Console.WriteLine("Fermeture de la liaison !!");
            p.Close();
            //raw_stream.Close();
        }

        private static void Traduction()
        {
            byte[] myBytes = raw_stream.GetBuffer(); //File.ReadAllBytes(Config.nom_fichier_8); //
            byte[] myBytes16MHz = Utils.Convert8To16Bit(myBytes);
            string raw_name = Config.nom_fichier_16 + ".raw";
            FileStream bytes_StreamBG = new FileStream(raw_name, FileMode.Create);
            bytes_StreamBG.Write(myBytes16MHz, 0, myBytes16MHz.Length);
            bytes_StreamBG.Close();
            System.IO.Directory.CreateDirectory(Config.outfolder);
            Utils.RunProcess(Config.switch_exe_path, Config.options_outfolder);
            Utils.RunProcess(Config.switch_exe_path, Config.options_switch);
            string flac_name = Config.nom_fichier_16 + ".flac";
            byte[] flac_Bytes = File.ReadAllBytes(Config.outfolder + "\\" + flac_name);
            var task = Utils.DoStuff(flac_Bytes);
            task.Wait();
            var content = JObject.Parse(task.Result);
            var hypotheses = content["hypotheses"];
            if (hypotheses != null && hypotheses.Count() > 0)
            {
                Console.WriteLine(Environment.NewLine + "Result: " + hypotheses.First["utterance"]);
                message = (string)hypotheses.First["utterance"];

            }
            Console.WriteLine("Traduction effectuée !!");
            Console.Read();
        }

        private static void Transmission()
        {
            SerialPort p = new SerialPort(Config.SerialPortParameters.nomPort, Config.SerialPortParameters.vitesse, Parity.None, 
                                    Config.SerialPortParameters.bitDonnee, StopBits.One);
            p.Open(); 
            Console.WriteLine(" ");
            Console.WriteLine("Transmission");
            Console.WriteLine(" ");
            p.Write(message);
            p.Close();
        }

        private static void DataReceivedHandler(
                     object sender,
                     SerialDataReceivedEventArgs e)
        {
            SerialPort sp = (SerialPort)sender;
            byte[] myBytes = new byte[sp.BytesToRead];
            sp.Read(myBytes, 0, myBytes.Length);
            Console.WriteLine("ecriture");
            raw_stream.Write(myBytes, 0, myBytes.Length);
        }

    }
}
