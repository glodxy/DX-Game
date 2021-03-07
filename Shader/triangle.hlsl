float4 VSMain(float4 Pos:POSITION):SV_POSITION{
    return Pos;
}

float4 PSMain(float4 Pos:POSITION):SV_TARGET{
    return float4(1.0f,0.0f,0.0f,1.0f);
}