using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CacheSimulator
{
    class CacheStatus
    {
        //Variables to show the status of the operation done in Cache
        public string access;
        public string address;
        public int tag;
        public int index;
        public int offset;
        public string result;
        public int memRefs;
    }
}
