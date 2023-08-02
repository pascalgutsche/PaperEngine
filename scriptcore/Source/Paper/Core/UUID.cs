using System;

namespace Paper
{
    public class UUID
    {
        private ulong _uuid;

        public UUID(ulong _UUID)
        {
            _uuid = _UUID;
        }

        public UUID(string _UUID)
        {
            _uuid = Convert.ToUInt64(_UUID, 16);
        }

        public UUID(UUID _UUID)
        {
            this._uuid = _UUID._uuid;
        }

        public void Set(ulong _UUID)
        {
            _uuid = _UUID;
        }

        public void Set(string _UUID)
        {
            _uuid = Convert.ToUInt64(_UUID, 16);
        }

        public override string ToString()
        {
            return _uuid.ToString("x16");
        }

        public ulong ToUlong()
        {
            return _uuid;
        }

        public static implicit operator ulong(UUID _UUID)
        {
            return _UUID._uuid;
        }

        public static implicit operator UUID(ulong _UUID)
        {
            return new UUID(_UUID);
        }

        public static implicit operator string(UUID _UUID)
        {
            return _UUID.ToString();
        }

        public static implicit operator UUID(string _UUID)
        {
            return new UUID(_UUID);
        }
    }
}