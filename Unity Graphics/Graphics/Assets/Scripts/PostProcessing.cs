using UnityEngine;

[RequireComponent(typeof(Camera))]
public class PostProcessing : MonoBehaviour
{
    Camera camera;

    public Material _mat;
    void Start()
    {
        camera = GetComponent<Camera>();
        camera.depthTextureMode = DepthTextureMode.Depth;
    }

    void Update()
    {
        
    }

    private void OnRenderImage(RenderTexture source, RenderTexture destination)
    {
        _mat.SetFloat("_DeltaX", 1.0f / (float)source.width);
        _mat.SetFloat("_DeltaY", 1.0f / (float)source.height);

        Graphics.Blit(source, destination, _mat);
    }
}
