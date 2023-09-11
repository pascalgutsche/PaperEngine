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
            get => InternalCalls.DataComponent_GetName(Entity.PaperID);
            set => InternalCalls.DataComponent_SetName(Entity.PaperID, value);
        }

        public PaperID PaperID => Entity.PaperID;

        public List<string> Tags
        {
            get
            {
                string tags = InternalCalls.DataComponent_GetTags(Entity.PaperID);
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
                InternalCalls.DataComponent_SetTags(Entity.PaperID, tags);
            }
        }
    }

    public class TransformComponent : Component
    {
        public Vec3 Position
        {
            get
            {
                InternalCalls.TransformComponent_GetPosition(Entity.PaperID, out Vec3 position);
                return position;
            }
            set
            {
                InternalCalls.TransformComponent_SetPosition(Entity.PaperID, ref value);
            }
        }

        public Vec3 Rotation
        {
            get
            {
                InternalCalls.TransformComponent_GetRotation(Entity.PaperID, out Vec3 rotation);
                return rotation;
            }
            set
            {
                InternalCalls.TransformComponent_SetRotation(Entity.PaperID, ref value);
            }
        }

        public Vec3 Scale
        {
            get
            {
                InternalCalls.TransformComponent_GetScale(Entity.PaperID, out Vec3 scale);
                return scale;
            }
            set
            {
                InternalCalls.TransformComponent_SetScale(Entity.PaperID, ref value);
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
                InternalCalls.SpriteComponent_GetColor(Entity.PaperID, out Vec4 color);
                return color;
            }
            set => InternalCalls.SpriteComponent_SetColor(Entity.PaperID, ref value);
        }

        public Texture Texture
        {
            get
            {
                InternalCalls.SpriteComponent_GetTexture(Entity.PaperID, out TextureData textureData);
                return new Texture(textureData);
            }
            set
            {
                string filePath = value.FilePath;
                InternalCalls.SpriteComponent_SetTexture(Entity.PaperID, filePath);
            }
        }

        public float TilingFactor
        {
            get => InternalCalls.SpriteComponent_GetTilingFactor(Entity.PaperID);
            set => InternalCalls.SpriteComponent_SetTilingFactor(Entity.PaperID, value);
        }

        public Vec2 UV0
        {
            get
            {
                InternalCalls.SpriteComponent_GetUV0(Entity.PaperID, out Vec2 uv0);
                return uv0;
            }
            set => InternalCalls.SpriteComponent_SetUV0(Entity.PaperID, ref value);
        }

        public Vec2 UV1
        {
            get
            {
                InternalCalls.SpriteComponent_GetUV1(Entity.PaperID, out Vec2 uv1);
                return uv1;
            }
            set => InternalCalls.SpriteComponent_SetUV1(Entity.PaperID, ref value);
        }

        public Geometry Geometry
        {
            get => (Geometry)InternalCalls.SpriteComponent_GetGeometry(Entity.PaperID);
            set => InternalCalls.SpriteComponent_SetGeometry(Entity.PaperID, (int)value);
        }

        public float Thickness
        {
            get => InternalCalls.SpriteComponent_GetThickness(Entity.PaperID);
            set => InternalCalls.SpriteComponent_SetThickness(Entity.PaperID, value);
        }

        public float Fade
        {
            get => InternalCalls.SpriteComponent_GetFade(Entity.PaperID);
            set => InternalCalls.SpriteComponent_SetFade(Entity.PaperID, value);
        }
    }

    public class LineComponent : Component
    {
        public Vec4 Color
        {
            get
            {
                InternalCalls.LineComponent_GetColor(Entity.PaperID, out Vec4 color);
                return color;
            }
            set => InternalCalls.LineComponent_SetColor(Entity.PaperID, ref value);
        }

        public float Thickness
        {
            get => InternalCalls.LineComponent_GetThickness(Entity.PaperID);
            set => InternalCalls.LineComponent_SetThickness(Entity.PaperID, value);
        }
    }

    public class TextComponent : Component
    {
        public Vec4 Color
        {
            get
            {
                InternalCalls.TextComponent_GetColor(Entity.PaperID, out Vec4 color);
                return color;
            }
            set => InternalCalls.TextComponent_SetColor(Entity.PaperID, ref value);
        }

        public string Text
        {
            get => InternalCalls.TextComponent_GetText(Entity.PaperID);
            set => InternalCalls.TextComponent_SetText(Entity.PaperID, value);
        }

        public string FontPath
        {
            get => InternalCalls.TextComponent_GetFontPath(Entity.PaperID);
            set => InternalCalls.TextComponent_SetFontPath(Entity.PaperID, value);
        }
    }

    public class CameraComponent : Component
    {
        public bool FixedAspectRatio
        {
            get => InternalCalls.CameraComponent_GetFixedAspectRatio(Entity.PaperID);
            set => InternalCalls.CameraComponent_SetFixedAspectRatio(Entity.PaperID, value);
        }

        public bool Primary
        {
            get => InternalCalls.CameraComponent_GetPrimary(Entity.PaperID);
            set => InternalCalls.CameraComponent_SetPrimary(Entity.PaperID, value);
        }

        public EntityCamera EntityCamera => new EntityCamera(Entity.PaperID);
    }

    public class ScriptComponent : Component
    {
        public string ScriptClassName
        {
            get => InternalCalls.ScriptComponent_GetScriptClassName(Entity.PaperID);
            set => InternalCalls.ScriptComponent_SetScriptClassName(Entity.PaperID, value);
        }
    }

    public class Rigidbody2DComponent : Component
    {
        public string ScriptClassName
        {
            get => InternalCalls.ScriptComponent_GetScriptClassName(Entity.PaperID);
            set => InternalCalls.ScriptComponent_SetScriptClassName(Entity.PaperID, value);
        }
    }

    public class Collider2DComponent : Component
    {
        public string ScriptClassName
        {
            get => InternalCalls.ScriptComponent_GetScriptClassName(Entity.PaperID);
            set => InternalCalls.ScriptComponent_SetScriptClassName(Entity.PaperID, value);
        }
    }
}