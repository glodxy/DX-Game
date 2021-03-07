cbuffer ConstantBuffer{
    matrix Model;
    matrix View;
    matrix Projection;
}

struct VertexIn
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
};


struct VertexOut
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

VertexOut VS(VertexIn input){
    VertexOut vout;
    vout.Pos=mul(Model,input.Pos);
    vout.Pos=mul(View,vout.Pos);
    vout.Pos=mul(Projection,vout.Pos);
    vout.Color=input.Color;
    return vout;
}

float4 PS(VertexOut input):SV_Target{
    return input.Color;
}

technique11 ColorTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}