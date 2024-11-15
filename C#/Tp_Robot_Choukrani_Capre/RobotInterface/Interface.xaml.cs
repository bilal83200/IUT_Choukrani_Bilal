using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO.Ports;
using System.Windows.Threading;

using ExtendedSerialPort_NS;
using Robot_NS;

namespace RobotInterface
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class Interface : Window
    {
        public DispatcherTimer timerAffichage;
        public ExtendedSerialPort serialPort1;
        public Robot robot = new Robot();

        public Interface()
        {
            InitializeComponent();
            serialPort1 = new ExtendedSerialPort("COM6", 115200, Parity.None, 8, StopBits.One);
            serialPort1.DataReceived += SerialPort1_DataReceived;
            serialPort1.Open();

            timerAffichage = new DispatcherTimer();
            timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 100);
            timerAffichage.Tick += TimerAffichage_Tick;
            timerAffichage.Start();

        }

        private void TimerAffichage_Tick(object? sender, EventArgs e)
        {
            textboxReception.Text += robot.receivedText;
            robot.receivedText = "";
        }

        bool toggle = false;
        bool suppr = false;
        public void SerialPort1_DataReceived(object? sender, DataReceivedArgs e)
        {
            robot.receivedText = Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);
        }

        private void buttonEnvoyer_Click(object sender, RoutedEventArgs e)
        {
            if (toggle == true)
            {
                buttonEnvoyer.Background = Brushes.RoyalBlue;
                toggle = false;
            }
            else
            {
                buttonEnvoyer.Background = Brushes.Beige;
                toggle = true;
            }

            SendMessage();

        }
        private void buttonSupprimer_Click(object sender, RoutedEventArgs e)
        {
            if (suppr == true)
            {
                buttonSupprimer.Background = Brushes.RoyalBlue;
                suppr = false;
            }
            else
            {
                buttonSupprimer.Background = Brushes.Beige;
                suppr = true;
            }

            ClearMessage();
        }

        private void SendMessage()
        {
            textboxReception.Text += "reçu :" + textboxEmission.Text + "\n";
            String val = robot.receivedText;
            serialPort1.WriteLine(val);
            textboxEmission.Text = "";
        }

        private void ClearMessage()
        {
            textboxReception.Text = "";
        }
        private void textboxEmission_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                SendMessage();
            }


        }

        private void buttonTest_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}