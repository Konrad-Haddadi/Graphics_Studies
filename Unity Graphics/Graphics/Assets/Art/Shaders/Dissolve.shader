Shader "Custom/Dissolve"
{
    Properties
    {
        _Color ("Color", Color) = (1,1,1,1)
        _MainTex ("Albedo (RGB)", 2D) = "white" {}
        _Glossiness ("Smoothness", Range(0,1)) = 0.5
        _Metallic ("Metallic", Range(0,1)) = 0.0
        _DissolveAmount("Dissolve Amount", Range(1, 10)) = 2
        _TimerSpeed("Timer Speed", Range(1, 10)) = 2
        _PerlinNoise("Perlin Noise", 2D) = "white" {}
    }
    SubShader
    {
        Tags { "RenderType"="Opaque" }
        LOD 200

        CGPROGRAM
        // Physically based Standard lighting model, and enable shadows on all light types
        #pragma surface surf Standard fullforwardshadows

        // Use shader model 3.0 target, to get nicer looking lighting
        #pragma target 3.0

        sampler2D _MainTex;

        struct Input
        {
            float2 uv_MainTex;
            float3 viewDir;
        };

        half _Glossiness;
        half _Metallic;
        fixed4 _Color;
        float _DissolveAmount;
        float _TimerSpeed;
        

        // Add instancing support for this shader. You need to check 'Enable Instancing' on materials that use the shader.
        // See https://docs.unity3d.com/Manual/GPUInstancing.html for more information about instancing.
        // #pragma instancing_options assumeuniformscaling
        UNITY_INSTANCING_BUFFER_START(Props)
            // put more per-instance properties here
        UNITY_INSTANCING_BUFFER_END(Props)

        void surf (Input IN, inout SurfaceOutputStandard o)
        {
            
            // Albedo comes from a texture tinted by color
            fixed4 c = tex2D(_MainTex, IN.uv_MainTex) * _Color;
        
            //float xPos = IN.uv_MainTex.x ;
            //float yPos = IN.uv_MainTex.y ;
            

            //clip(sin(xPos * _DissolveAmount + _Time.z * _TimerSpeed));
            //clip(sin(yPos * _DissolveAmount + _Time.z * _TimerSpeed));
            
            float xPos = c.x;
            float yPos = c.y;
            

            clip(sin(xPos * _DissolveAmount + _Time.z * _TimerSpeed));
            clip(sin(yPos * _DissolveAmount + _Time.w * _TimerSpeed));

            o.Albedo = c.rgb;

            // Metallic and smoothness come from slider variables
            o.Metallic = _Metallic;
            o.Smoothness = _Glossiness;
            o.Alpha = c.a;           
        }
        ENDCG
    }
    FallBack "Diffuse"
}
