using System;
using System.Collections.Generic;
using System.Text;

namespace Paper
{
    public abstract class Component
    {
        internal Entity Entity { get; set; }
    }

    public class DataComponent : Component
    {
        public string Name
        {
            get => InternalCalls.DataComponent_GetName(Entity.UUID);
            set => InternalCalls.DataComponent_SetName(Entity.UUID, value);
        }

        public UUID UUID => Entity.UUID;

        public List<string> Tags
        {
            get
            {
                string tags = InternalCalls.DataComponent_GetTags(Entity.UUID);
                List<string> tagList = new List<string>();
                tagList.AddRange(tags.Split(','));
                return tagList;
            }
            set
            {
                string tags = "";
                List<string> tagList = value;
                foreach (string tag in tagList)
                {
                    tags += tag;
                    tags += ',';
                }
                if (tags.Length > 0)
                    tags = tags.Remove(tags.Length - 1, 1);
                InternalCalls.DataComponent_SetTags(Entity.UUID, tags);
            }
        }
    }

    public class TransformComponent : Component
    {
        public Vec3 Position
        {
            get
            {
                InternalCalls.TransformComponent_GetPosition(Entity.UUID, out Vec3 position);
                return position;
            }
            set
            {
                InternalCalls.TransformComponent_SetPosition(Entity.UUID, ref value);
            }
        }

        public Vec3 Rotation
        {
            get
            {
                InternalCalls.TransformComponent_GetRotation(Entity.UUID, out Vec3 rotation);
                return rotation;
            }
            set
            {
                InternalCalls.TransformComponent_SetRotation(Entity.UUID, ref value);
            }
        }

        public Vec3 Scale
        {
            get
            {
                InternalCalls.TransformComponent_GetScale(Entity.UUID, out Vec3 scale);
                return scale;
            }
            set
            {
                InternalCalls.TransformComponent_SetScale(Entity.UUID, ref value);
            }
        }
    }

    public enum Geometry
    {
        None,
        Triangle,
        Rectangle,
        Circle
    }

    public class SpriteComponent : Component
    {
        public Vec4 Color
        {
            get
            {
                InternalCalls.SpriteComponent_GetColor(Entity.UUID, out Vec4 color);
                return color;
            }
            set => InternalCalls.SpriteComponent_SetColor(Entity.UUID, ref value);
        }

        public Texture Texture
        {
            get
            {
                InternalCalls.SpriteComponent_GetTexture(Entity.UUID, out TextureData textureData);
                return new Texture(textureData);
            }
            set
            {
                string filePath = value.FilePath;
                InternalCalls.SpriteComponent_SetTexture(Entity.UUID, filePath);
            }
        }

        public float TilingFactor
        {
            get => InternalCalls.SpriteComponent_GetTilingFactor(Entity.UUID);
            set => InternalCalls.SpriteComponent_SetTilingFactor(Entity.UUID, value);
        }

        public Vec2 UV0
        {
            get
            {
                InternalCalls.SpriteComponent_GetUV0(Entity.UUID, out Vec2 uv0);
                return uv0;
            }
            set => InternalCalls.SpriteComponent_SetUV0(Entity.UUID, ref value);
        }

        public Vec2 UV1
        {
            get
            {
                InternalCalls.SpriteComponent_GetUV1(Entity.UUID, out Vec2 uv1);
                return uv1;
            }
            set => InternalCalls.SpriteComponent_SetUV1(Entity.UUID, ref value);
        }

        public Geometry Geometry
        {
            get => (Geometry)InternalCalls.SpriteComponent_GetGeometry(Entity.UUID);
            set => InternalCalls.SpriteComponent_SetGeometry(Entity.UUID, (int)value);
        }

        public float Thickness
        {
            get => InternalCalls.SpriteComponent_GetThickness(Entity.UUID);
            set => InternalCalls.SpriteComponent_SetThickness(Entity.UUID, value);
        }

        public float Fade
        {
            get => InternalCalls.SpriteComponent_GetFade(Entity.UUID);
            set => InternalCalls.SpriteComponent_SetFade(Entity.UUID, value);
        }
    }

    public class LineComponent : Component
    {
        public Vec4 Color
        {
            get
            {
                InternalCalls.LineComponent_GetColor(Entity.UUID, out Vec4 color);
                return color;
            }
            set => InternalCalls.LineComponent_SetColor(Entity.UUID, ref value);
        }

        public float Thickness
        {
            get => InternalCalls.LineComponent_GetThickness(Entity.UUID);
            set => InternalCalls.LineComponent_SetThickness(Entity.UUID, value);
        }
    }

    public class TextComponent : Component
    {
        public Vec4 Color
        {
            get
            {
                InternalCalls.TextComponent_GetColor(Entity.UUID, out Vec4 color);
                return color;
            }
            set => InternalCalls.TextComponent_SetColor(Entity.UUID, ref value);
        }

        public string Text
        {
            get => InternalCalls.TextComponent_GetText(Entity.UUID);
            set => InternalCalls.TextComponent_SetText(Entity.UUID, value);
        }

        public string FontPath
        {
            get => InternalCalls.TextComponent_GetFontPath(Entity.UUID);
            set => InternalCalls.TextComponent_SetFontPath(Entity.UUID, value);
        }
    }
}