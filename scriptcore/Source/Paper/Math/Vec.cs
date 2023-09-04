namespace Paper
{
    public struct Vec2
    {
        public float X;
        public float Y;

        public Vec2(float _X, float _Y)
        {
            X = _X;
            Y = _Y;
        }
        public Vec2(float _Scalar)
        {
            X = _Scalar;
            Y = _Scalar;
        }
        public Vec2(Vec2 _Vec2)
        {
            X = _Vec2.X;
            Y = _Vec2.Y;
        }
        public Vec2(Vec3 _Vec3)
        {
            X = _Vec3.X;
            Y = _Vec3.Y;
        }
        public Vec2(Vec4 _Vec4)
        {
            X = _Vec4.X;
            Y = _Vec4.Y;
        }
    }

    public struct Vec3
    {
        public float X;
        public float Y;
        public float Z;

        public Vec3(float _X, float _Y, float _Z)
        {
            X = _X;
            Y = _Y;
            Z = _Z;
        }
        public Vec3(float _Scalar)
        {
            X = _Scalar;
            Y = _Scalar;
            Z = _Scalar;
        }
        public Vec3(Vec2 _Vec2)
        {
            X = _Vec2.X;
            Y = _Vec2.Y;
            Z = 0.0f;
        }
        public Vec3(Vec3 _Vec3)
        {
            X = _Vec3.X;
            Y = _Vec3.Y;
            Z = _Vec3.Z;
        }
        public Vec3(Vec4 _Vec4)
        {
            X = _Vec4.X;
            Y = _Vec4.Y;
            Z = _Vec4.Z;
        }
    }

    public struct Vec4
    {
        public float X;
        public float Y;
        public float Z;
        public float W;

        public Vec4(float _X, float _Y, float _Z, float _W)
        {
            X = _X;
            Y = _Y;
            Z = _Z;
            W = _W;
        }
        public Vec4(float _Scalar)
        {
            X = _Scalar;
            Y = _Scalar;
            Z = _Scalar;
            W = _Scalar;
        }
        public Vec4(Vec2 _Vec2)
        {
            X = _Vec2.X; 
            Y = _Vec2.Y;
            Z = 0.0f;
            W = 0.0f;
        }
        public Vec4(Vec3 _Vec3)
        {
            X = _Vec3.X;
            Y = _Vec3.Y;
            Z = _Vec3.Z;
            W = 0.0f;
        }
        public Vec4(Vec4 _Vec4)
        {
            X = _Vec4.X;
            Y = _Vec4.Y;
            Z = _Vec4.Z;
            W = _Vec4.W;
        }
    }
}