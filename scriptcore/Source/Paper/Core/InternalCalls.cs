using System;
using System.Runtime.CompilerServices;

namespace Paper
{
    internal static class InternalCalls
    {
        #region Input

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern bool IsKeyPressed(int _Code);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern bool IsKeyHeld(int _Code);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern bool IsKeyDown(int _Code);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern bool IsKeyReleased(int _Code);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern bool IsMouseButtonPressed(int _Button);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern bool IsMouseButtonHeld(int _Button);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern bool IsMouseButtonDown(int _Button);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern bool IsMouseButtonReleased(int _Button);

        #endregion

        #region Render

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void GetTexture(string _FilePath, out TextureData _Data);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern float EntityCamera_GetPerspectiveFOV(ulong _EntityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void EntityCamera_SetPerspectiveFOV(ulong _EntityID, float _FOV);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern float EntityCamera_GetPerspectiveNearClip(ulong _EntityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void EntityCamera_SetPerspectiveNearClip(ulong _EntityID, float _NearClip);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern float EntityCamera_GetPerspectiveFarClip(ulong _EntityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void EntityCamera_SetPerspectiveFarClip(ulong _EntityID, float _FarCLip);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern float EntityCamera_GetOrthographicSize(ulong _EntityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void EntityCamera_SetOrthographicSize(ulong _EntityID, float _Size);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern float EntityCamera_GetOrthographicNearClip(ulong _EntityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void EntityCamera_SetOrthographicNearClip(ulong _EntityID, float _NearClip);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern float EntityCamera_GetOrthographicFarClip(ulong _EntityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void EntityCamera_SetOrthographicFarClip(ulong _EntityID, float _FarClip);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void EntityCamera_SetViewportSize(ulong _EntityID, ref Vec2 _Size);

        #endregion

        #region Entity

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern bool Entity_HasComponent(ulong _UUID, Type _ComponentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern ulong Entity_GetEntityByName(string _Name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern object Entity_GetScriptInstance(ulong _UUID);


        #endregion

        #region Components

        #region TransformCompoonent

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void TransformComponent_GetPosition(ulong _UUID, out Vec3 _Position);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void TransformComponent_SetPosition(ulong _UUID, ref Vec3 _Position);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void TransformComponent_GetRotation(ulong _UUID, out Vec3 _Rotation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void TransformComponent_SetRotation(ulong _UUID, ref Vec3 _Rotation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void TransformComponent_GetScale(ulong _UUID, out Vec3 _Scale);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void TransformComponent_SetScale(ulong _UUID, ref Vec3 _Scale);

        #endregion

        #region DataComponent

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern string DataComponent_GetName(ulong _UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void DataComponent_SetName(ulong _UUID, string _Name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern string DataComponent_GetTags(ulong _UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void DataComponent_SetTags(ulong _UUID, string _Tags);

        #endregion

        #region SpriteComponent

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SpriteComponent_GetColor(ulong _UUID, out Vec4 _Color);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SpriteComponent_SetColor(ulong _UUID, ref Vec4 _Color);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SpriteComponent_GetTexture(ulong _UUID, out TextureData _TextureData);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SpriteComponent_SetTexture(ulong _UUID, string _FilePath);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern float SpriteComponent_GetTilingFactor(ulong _UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SpriteComponent_SetTilingFactor(ulong _UUID, float _TilingFactor);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SpriteComponent_GetUV0(ulong _UUID, out Vec2 _UV0);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SpriteComponent_SetUV0(ulong _UUID, ref Vec2 _UV0);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SpriteComponent_GetUV1(ulong _UUID, out Vec2 _UV1);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SpriteComponent_SetUV1(ulong _UUID, ref Vec2 _UV1);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern int SpriteComponent_GetGeometry(ulong _UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SpriteComponent_SetGeometry(ulong _UUID, int _Geometry);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern float SpriteComponent_GetThickness(ulong _UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SpriteComponent_SetThickness(ulong _UUID, float _Thickness);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern float SpriteComponent_GetFade(ulong _UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SpriteComponent_SetFade(ulong _UUID, float _Fade);

        #endregion

        #region LineComponent

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void LineComponent_GetColor(ulong _UUID, out Vec4 _Color);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void LineComponent_SetColor(ulong _UUID, ref Vec4 _Color);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern float LineComponent_GetThickness(ulong _UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void LineComponent_SetThickness(ulong _UUID, float _Thickness);

        #endregion

        #region TextComponent

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void TextComponent_GetColor(ulong _UUID, out Vec4 _Color);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void TextComponent_SetColor(ulong _UUID, ref Vec4 _Color);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern string TextComponent_GetText(ulong _UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void TextComponent_SetText(ulong _UUID, string _Text);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern string TextComponent_GetFontPath(ulong _UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void TextComponent_SetFontPath(ulong _UUID, string _FontPath);

        #endregion

        #region Rigidbody2DComponent

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Rigidbody2DComponent_ApplyImpulse(ulong _UUID, ref Vec2 _Impulse, bool _Wake);

        #endregion

        #region CameraComponent

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern bool CameraComponent_GetFixedAspectRatio(ulong _EntityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void CameraComponent_SetFixedAspectRatio(ulong _EntityID, bool _FixedAspectRatio);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern bool CameraComponent_GetPrimary(ulong _EntityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void CameraComponent_SetPrimary(ulong _EntityID, bool _Primary);

        #endregion

        #region ScriptComponent

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern string ScriptComponent_GetScriptClassName(PaperID _EntityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void ScriptComponent_SetScriptClassName(PaperID _EntityID, string _ScriptClassName);

        #endregion

        #endregion



    }
}
