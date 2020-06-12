﻿using System; 
using System.Windows; 
using System.Windows.Media; 
using dasData;
using System.Windows.Threading;
using Newtonsoft.Json;
using System.IO;
using System.Net;
using System.Collections.ObjectModel;
using Syncfusion.UI.Xaml.Charts;
using System.Data;
using System.Threading.Tasks;
using System.Timers;

namespace MultiSenseCrown
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public ObservableCollection<VitalData> Data { get; set; }
        public ObservableCollection<BodyTemp> BodyData  { get; set; }
        public ObservableCollection<OxygenSaturation> OxyData { get; set; }
        public ObservableCollection<BodyPulse> HeartData { get; set; }

        private DateTime timestamp = DateTime.Now;  
        private static int intervalRead = 1000;
        private DispatcherTimer dispatcherTimer;

        private string _domain = "https://dasdata.co/";
        private string _dasToken = "&das=cv+L1FGsSVaOptLc/yk4zAq367n5yqFqP6X4CbAqoNRv4a27VVX9RZB2jllGPur0GHDpQaG8C9UnU75NR7lWZw==";


        public MainWindow()
        {
            InitializeComponent();

            // get the latest   
            cmdGetData("latest", "0", "30"); 
            // read every 3 seconds 
            dispatcherTimer = new System.Windows.Threading.DispatcherTimer();
            dispatcherTimer.Tick += new EventHandler(dispatcherTimer_Tick);
            dispatcherTimer.Interval = new TimeSpan(0, 0, 3);
            dispatcherTimer.Start(); 
        }

        private void dispatcherTimer_Tick(object sender, EventArgs e)
        {  
            string format = "dd.MM.yyyy";
            string selectedDate = timestamp.ToString(format);
            lblDateTimeNow.Content = selectedDate; 
            cmdGetData("latest", "0", "30");
        } 

        // get data and bind components
        private void cmdGetData(string _type, string _from, string _to) {
            try
            {
                string _urldata = "";  
                if (_type == "interval") {
                      _urldata = _domain + "/e.aspx?json=1&dfrom=" + _from + "&dto=" + _to + _dasToken;
                } else if (_type == "latest") {
                      _urldata = _domain + "/e.aspx?json=1&from=" + _from + "&to=" + _to + _dasToken;
                } 
            
                WebRequest request = WebRequest.Create(_urldata);
                request.Credentials = CredentialCache.DefaultCredentials;
                WebResponse response = request.GetResponse(); 
                Stream dataStream = response.GetResponseStream();
                StreamReader reader = new StreamReader(dataStream);
                string responseFromServer = reader.ReadToEnd();
                // txtResult.Text = (responseFromServer);

                reader.Close();
                response.Close();

                DataSet dataSet = JsonConvert.DeserializeObject<DataSet>(responseFromServer);
                DataTable dataTable = dataSet.Tables[0];
                dataTable.DefaultView.Sort = "id ASC"; 
                dataTable = dataTable.DefaultView.ToTable(); // rearange to the latest

                DataColumnCollection columns = dataTable.Columns;

                Data = new ObservableCollection<VitalData>();
                BodyData = new ObservableCollection<BodyTemp>();
                OxyData = new ObservableCollection<OxygenSaturation>();
                HeartData = new ObservableCollection<BodyPulse>();

                if (dataTable.Rows.Count < 1) {
                    MessageBox.Show("No data available.");
                    return;
                } 

                int i = 0;
                foreach (DataRow row in dataTable.Rows)
                {
                    foreach (DataColumn column in dataTable.Columns)
                    {  
                        if (column.ColumnName == "AIDate")
                        {
                            if (i == dataTable.Rows.Count-1) { 
                               lblLatestUpdate.Content = row[column].ToString();
                                DateTime now = DateTime.Now;
                                DateTime updated =   DateTime.Parse(row[column].ToString()).AddHours(-1); // server time is -1 hour CET
                                TimeSpan updatespan = now.Subtract(updated);
                                TimeSpan e5sec = TimeSpan.FromSeconds(5.0); 

                                lblInfo.Content = updatespan +" "+ e5sec.ToString(); 
                                if (updatespan < e5sec)
                                {
                                    lblStatusCrown.Content = "CROWN ON HEAD"; crownSign.Visibility = Visibility.Visible;
                                    imgHuman.Opacity = 1;
                                }
                                else
                                {
                                    lblStatusCrown.Content = "CROWN NOT MOUNTED"; crownSign.Visibility = Visibility.Hidden;
                                    imgHuman.Opacity = 0.8;
                                }
                            }
                           
                            timestamp = DateTime.Parse(row[column].ToString());
                        }

                        if (column.ColumnName == "TemperatureBody")
                        {
                            if (i == dataTable.Rows.Count-1)
                            {
                                cpbBodyTemp.Progress = Convert.ToDouble(row[column].ToString());
                                // latestTemp = Convert.ToDouble(row[column].ToString()); 
                            }
                            BodyData.Add(new BodyTemp() { Timestamp = timestamp, TempVal = Convert.ToDouble(row[column].ToString()) });
                        }

                        if (column.ColumnName == "OxygenSaturation")
                        {
                            if (i == dataTable.Rows.Count -1)
                            {
                                cpbBodyOxygen.Progress = Convert.ToDouble(row[column].ToString());
                            }
                            OxyData.Add(new OxygenSaturation() { Timestamp = timestamp, OxygenVal = Convert.ToDouble(row[column].ToString()) });
                        }

                        if (column.ColumnName == "HeartRate")
                        {
                            if (i == dataTable.Rows.Count-1)
                            {
                                cpbBodyPulse.Progress = Convert.ToDouble(row[column].ToString());
                            }
                            HeartData.Add(new BodyPulse() { Timestamp = timestamp, PulseVal = Convert.ToDouble(row[column].ToString()) });
                        }

                    }
                    i += 1;
                } 



                SparkGridTemp.Children.Clear();

                SfLineSparkline sparkline = new SfLineSparkline()
                {
                    ItemsSource = BodyData,
                    YBindingPath = "TempVal",
                    BandRangeStart =38,
                    BandRangeEnd = 41,
                    RangeBandBrush = new SolidColorBrush(Colors.Red),
                    MaximumYValue = 42,
                    MinimumYValue = 34,
                    ShowTrackBall = false
                };
                this.SparkGridTemp.Children.Add(sparkline);

                SparkGridOxygen.Children.Clear();
                SfLineSparkline sparklineOxy = new SfLineSparkline()
                {
                    ItemsSource = OxyData,
                    YBindingPath = "OxygenVal",
                    BandRangeStart = 90,
                    BandRangeEnd = 94,
                    RangeBandBrush = new SolidColorBrush(Colors.Red),
                    MaximumYValue = 90,
                    MinimumYValue = 100,
                    ShowTrackBall = false
                }; 
                this.SparkGridOxygen.Children.Add(sparklineOxy);

                SparkGridHeart.Children.Clear();
                SfLineSparkline sparklineHeart = new SfLineSparkline()
                {
                    ItemsSource = HeartData,
                    YBindingPath = "PulseVal", 
                    BandRangeStart = 140,
                    BandRangeEnd = 180,
                    RangeBandBrush = new SolidColorBrush(Colors.Red),
                    MaximumYValue = 200,
                    MinimumYValue = 40,
                    ShowTrackBall = false
                };
                this.SparkGridHeart.Children.Add(sparklineHeart);

            }
            catch (Exception e)
            { 
                lblInfo.Content = e.Message.ToString();
            }
        }

 
 



        // when data is changed get day interval data
        private void dtPicker_SelectedDateChanged(object sender, System.Windows.Controls.SelectionChangedEventArgs e)
        {
            DateTime dtSelection = (DateTime)dtPicker.SelectedDate;
            string format = "dd.MM.yyyy";
            string selectedDate = dtSelection.ToString(format);
            lblDateTimeNow.Content = selectedDate;

            imgHuman.Opacity = 1;
            cmdGetData("interval", selectedDate, selectedDate);

            // stop the 3 seconds counter as well 
            dispatcherTimer.Stop();

        }
    }

}
