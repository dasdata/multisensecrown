using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MultiSenseCrown
{
   public class VitalData
    {
        public DateTime Timestamp { get; set; } 
        public double TempVal { get; set; }
        public double OxygenVal { get; set; }
        public double PulseVal { get; set; }
    
    }
}
