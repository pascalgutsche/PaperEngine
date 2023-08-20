using System;

namespace Paper
{
    public class PaperID
    {
        private ulong _id;

        public PaperID(ulong _ID)
        {
            _id = _ID;
        }

        public PaperID(string _UUID)
        {
            _id = Convert.ToUInt64(_UUID, 16);
        }

        public PaperID(PaperID _PaperID)
        {
            this._id = _PaperID._id;
        }

        public void Set(ulong _UUID)
        {
            _id = _UUID;
        }

        public void Set(string _UUID)
        {
            _id = Convert.ToUInt64(_UUID, 16);
        }

        public override string ToString()
        {
            return _id.ToString("x16");
        }

        public ulong ToUlong()
        {
            return _id;
        }

        public static implicit operator ulong(PaperID _PaperID)
        {
            return _PaperID.ToUlong();
        }

        public static implicit operator PaperID(ulong _ID)
        {
            return new PaperID(_ID);
        }

        public static implicit operator string(PaperID _PaperID)
        {
            return _PaperID.ToString();
        }

        public static implicit operator PaperID(string _ID)
        {
            return new PaperID(_ID);
        }
    }
}