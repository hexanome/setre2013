using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TraductionAudioTexte
{
    class Config
    {
        public struct SerialPortParameters
        {
            public static int nbBytes = 30;
            public static String nomPort = "COM7";
            public static int vitesse = 57600;
            public static int bitDonnee = 8;
        }
        public static string nom_fichier_8 = "audio_bin2";// fichier qui contient les données en 8 bit
        public static string nom_fichier_16 = "bytes_16";// fichier qui contient les données en 16 bit 
        public static string switch_exe_path = "C:\\Program Files (x86)\\NCH Software\\Switch\\switch.exe";// switch pour convertir le .raw en .flac
        public static string options_switch = "-hide -convert " + nom_fichier_16 + ".raw -format .flac -settings .flac 8 16000 1 -overwrite ALWAYS -hide";// options de conversion
        public static string options_outfolder = "-hide -outfolder C:\\TraductionAudioTexte";// setter le dossier du fichier de sortie de la conversion dans switch
        public static string outfolder = "C:\\TraductionAudioTexte";// dossier du fichier de sortie de la conversion
    }
}
