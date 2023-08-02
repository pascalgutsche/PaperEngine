using System;
using System.IO;
using System.Runtime.InteropServices;

namespace Paper
{
    [StructLayout(LayoutKind.Sequential)]
    internal struct TextureData
    {
        internal string filepath;
        internal uint width;
        internal uint height;
        internal uint ID;
    }

    public class Texture
    {
        public string FilePath { get; }
        public UInt32 TextureID { get; }
        public UInt32 Width { get; }
        public UInt32 Height { get; }


        public Texture(string _FilePath)
        {
            InternalCalls.GetTexture(_FilePath, out TextureData data);
            FilePath = data.filepath;
            TextureID = data.ID;
            Width = data.width;
            Height = data.height;
        }

        internal Texture(TextureData _Data)
        {
            FilePath = _Data.filepath;
            TextureID = _Data.ID;
            Width = _Data.width;
            Height = _Data.height;
        }
        public static bool operator==(Texture _First, Texture _Second)
        {
            if (_First == null || _Second == null) return false;
            return _First.TextureID == _Second.TextureID;
        }
        public static bool operator!=(Texture _First, Texture _Second)
        {
            if (_First == null || _Second == null) return false;
            return _First.TextureID != _Second.TextureID;
        }

        protected bool Equals(Texture _Other)
        {
            return TextureID == _Other.TextureID;
        }

        public override bool Equals(object _Obj)
        {
            if (ReferenceEquals(null, _Obj)) return false;
            if (ReferenceEquals(this, _Obj)) return true;
            if (_Obj.GetType() != this.GetType()) return false;
            return Equals((Texture)_Obj);
        }

        public override int GetHashCode()
        {
            return (int)TextureID;
        }
    }
}