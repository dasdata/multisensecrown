   M S B A M L   `   `   `  ����J  FMultiSenseCrown, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null1clr-namespace:MultiSenseCrownMultiSenseCrown  5   	   g cSyncfusion.SfSmithChart.WPF, Version=18.1460.0.42, Culture=neutral, PublicKeyToken=3d67ed1f87d44c89rPclr-namespace:Syncfusion.UI.Xaml.SmithChart;assembly=Syncfusion.SfSmithChart.WPFSyncfusion.UI.Xaml.SmithChart 5   9        MultiSenseCrown.MainWindow   5   	   W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   @d2http://schemas.microsoft.com/expression/blend/2008     5   	   Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5   	   /localclr-namespace:MultiSenseCrown     5   	   h dSyncfusion.SfProgressBar.WPF, Version=18.1460.0.42, Culture=neutral, PublicKeyToken=3d67ed1f87d44c89b ^Syncfusion.SfChart.WPF, Version=18.1460.0.42, Culture=neutral, PublicKeyToken=3d67ed1f87d44c89b ^Syncfusion.SfGauge.WPF, Version=18.1460.0.42, Culture=neutral, PublicKeyToken=3d67ed1f87d44c891chart!http://schemas.syncfusion.com/wpf    5   	   e
SmithChartPclr-namespace:Syncfusion.UI.Xaml.SmithChart;assembly=Syncfusion.SfSmithChart.WPF    69   -   5          �Name$  mainFrm��5   �    � Title$ MultiSenseCrown��5	   	   $��605.5��6!   $��928.794��60    � SizeToContent$ Width=�6@    � 
ResizeMode$ NoResize=�6V   
���   �.��5      � $��122,70,-6,-19q�6    � DataContext 5   
      MultiSenseCrown.BodyTemp  5      6     � RowDefinitions 5   
   �� 5      $��2* �6   �� 5      $
��261* �6    � ColumnDefinitions 5   
   �� 5      .+�5   
   �� -   5         ��Name$ imgHuman��5      $
��Left=�6"   $	��578��6=   $	��Top=�6J   $	��933��6b   $+��%MultiSenseCrown-STInterface-human.png��6n   $
��None=�6�   $��-126,-78,0,0q�6�   $��2��6�   �� 5   
   -   5         ��Name$ lblDateTimeNow��5      $��MultiSenseCrown��6(   $
��Left=�6B   ���	#4CFFFFFF$��-104,442,0,0q�6u   $��20(�6�   	 J� RenderTransformOrigin$	 0.548,-0.427&�6�   $	��Top=�6�   $��38��6�   $��1��6�   �� 5   
   $
��Left=�6   $	��101��60   $��-87,289,0,0q�6=   $	��Top=�6R   $	��210��6j   $��1��6v   .+�5      �� -   5        $ lblRoomTemp��5      $��Room Temp: 21'C��6)   $
��Left=�6C   ���	#FF3BAEF4$��18(�6v   �� 5      -   5        $ lblRoomHumd��5      $��Room Humid: 45%��6)   $
��Left=�6C   ���	#FF3BAEF4$��18(�6v   �� 5      -   5        $ lblRoomAirQuality��5      $��Air Quality: Fine��6/   $
��Left=�6K   ���	#FF3BAEF4$��18(�6~   5      �� 5   
   -   5        
 ��Name$
 CircularProGress��5      $
��Left=�6/   $	��312��6J   $��
484,18,0,0q�6W   $	��Top=�6k   $	��246��6�   $��1��6�   :  4Syncfusion.UI.Xaml.ProgressBar.SfCircularProgressBar  5!   
   -   5          Name$ cpbBodyTemp��5!   &      
StartAngle$	 178Q�6;      EndAngle$	 360Q�6L      IndicatorOuterRadius$
 0.35Q�6[      IndicatorInnerRadius$
 0.62Q�6w      TrackOuterRadius$	 0.6Q�6�      TrackInnerRadius$
 0.65Q�6�   $
��Left=�6�   $	��Top=�6�   $��	32,56,0,0q�6�   $	��194��6	  $	��140��6  $��25(�6"  &   System.Windows.Media.TextOptions   TextHintingMode$ Fixed=�60  4  .Syncfusion.UI.Xaml.ProgressBar.ProgressBarBase   Progress$ 36Q�6T     Minimum$ 33Q�6b     Maximum$ 41Q�6o  $��1��6|     
TrackColor 5"      �� 5#       �� Color$ 	#FF353F4A��6"    �� Opacity$ 1Q�64      RangeColors 5&      9  3Syncfusion.UI.Xaml.ProgressBar.RangeColorCollection  /  )Syncfusion.UI.Xaml.ProgressBar.RangeColor  5'         Color$
 Blue��6(      Start$ 30Q�65   
   End$ 35Q�6@     5(      $ Green��6(   $ 35Q�66   $
 37.5Q�6A     5)      $	 Red��6(   $
 37.5Q�64   $ 45Q�6A   5*      5,        5-   
   -	   5        $ cpbBodyOxygen��5-   &   $	 180Q�6=   $	 360Q�6N   $
 0.35Q�6_   $
 0.62Q�6{   $	 0.6Q�6�   $
 0.65Q�6�   $
��Left=�6�   $	��Top=�6�   $��
517,18,0,0q�6�   $	��194��6  $	��152��6  $��25(�6(  $ Fixed=�66  $ 36Q�6Z  $ 80Q�6h  $��1��6u  $ 99Q�6�   5.      �� 5/      $ 	#FF353F4A��6"   $ 1Q�64    52          53      $
 Blue��6(   $ 94Q�65   $	 100Q�6@     54      $ Green��6(   $ 92Q�66   $ 94Q�6A     55      $	 Red��6(   $ 80Q�64   $ 92Q�6?   56      58        5:   
   -
   5        $ cpbBodyPulse��5:   &   $	 178Q�6<   $	 360Q�6M   $
 0.35Q�6\   $
 0.62Q�6x   $	 0.6Q�6�   $
 0.65Q�6�   $
��Left=�6�   $	��Top=�6�   $��517,227,0,0q�6�   $	��194��6  $	��141��6  $��1��6'   5;      �� 5<      $ 	#FF353F4A��6"   $ 1Q�64    5?          5@      $
 Blue��6(   $ 40Q�65   $ 60Q�6@     5A      $ Green��6(   $ 60Q�66   $	 110Q�6A     5B      $ Yellow��6(   $	 110Q�67   $	 145Q�6C     5C      $	 Red��6(   $	 145Q�64   $	 190Q�6@   5D      5F      �� 5I   
   -   5        $ lblTemperature��5I      $��Body Temperature��6(   $
��Left=�6C   ���	#FF3BAEF4$��18(�6v   $��32,62,0,390q�6�   $��1��6�   �� 5J   
   -   5        $ lblPulse��5J      $��Body Heart Rate��6"   $
��Left=�6<   ���	#FF3BAEF4$��18(�6o   $��530,225,0,229q�6}   $��1��6�   �� 5K   
   -   5        $ lblSO2��5K      $��Body Oxygen Saturation��6    $
��Left=�6A   ���	#FF3BAEF4$��18(�6t   $��489,24,0,430q�6�   $��1��6�   �� 5L   
   -   5         ��Name$ 	txtResult��5L      $
��Left=�6%   $��38��6@   $��
65,431,0,0q�6L    �� TextWrapping$
 Wrap=�6`   $	��Top=�6t   $	��717��6�   ���	#FFECECEC$��1��6�   ��6�   Z� ��6�   Z� 6�   � 5O   
   -   5         �Name$ SparkGridTemp��5O      $
��Left=�6&   $��69��6A   $��	0,186,0,0q�6M   $	��Top=�6`   $	��224��6x   $��1��6�   � 5P   
   -   5        $ SparkGridOxygen��5P      $
��Left=�6(   $��69��6C   $��489,143,0,0q�6O   $	��Top=�6d   $	��224��6|   $��1��6�   � 5Q   
   -   5        $ SparkGridHeart��5Q      $
��Left=�6'   $��69��6B   $��489,357,0,0q�6N   $	��Top=�6c   $	��224��6{   $��1��6�   �� 5U   
   -   5          ��Name$  	crownSign��5U      
���/���
���   �$��2��5W      ! �� RadiusX$! 20��5X      " �� RadiusY$" 20��6   $��312,41,406,461q�5Y      $��1��6   �� 5]   
   -   5        $ lblLatestUpdate��5]      $��Latest update��6)   $
��Left=�6A   
�������$��703,431,0,60q�6p   $��10(�6�   $��1��6�   �� 5^   
   -   5        $ 
lblVersion��5^      $$��MultiSenseCrown 0.21.2 Preview��6$   $
��Left=�6M   
�������$��618,-38,0,509q�6|   $��9(�6�   $	 0.548,-0.427&�6�   $��2��6�   �� 5_   
   -   5        $ lblStatusCrown��5_      $�� ��6(   $��Center=�63   ���	#FF3BAEF4$��10(�6h   $��252,-4,357,486q�6v   $	��198��6�   $��Center=�6�   $��2��6�   �� 5`   
   -   5        $ lblPatientID��5`      $��PATIENT ID: nano33��6&   $
��Left=�6C   ���	#FF3BAEF4$��14(�6v   $��288,469,0,18q�6�   $	��186��6�   $��ExtraCondensed&�6�   $��1��6�   �� 5a   
   -   5        $ lblInfo��5a      $��Label��6!   $
��Left=�61   $��599,431,0,0q�6L   $	��Top=�6a   $��23��6y   $��92��6�   ���	#FF00BDDA$��10(�6�   $��1��6�   (  "System.Windows.Controls.DatePicker  5b   
   -   5        #  Name$# dtPicker��5b      $
��Left=�6'   $��-114,-4,0,0q�6B   $	��Top=�6W   $��14(�6�   $��2��6�   $��29��6�   $	��135��6�   ��6�   Z� 6�   5g      5h      