attribute vec4 aPosition;//顶点坐标，在外部获取传递进来
attribute vec2 aTexCoord;//材质(纹理)顶点坐标
varying vec2 vTexCoord;//输出的材质(纹理)坐标，给片元着色器用

void main(){
    //纹理坐标转换，以左上角为原点的纹理坐标转换成以左下角为原点的纹理坐标
    //比如以左上角为原点的(0,0)对应以左下角为原点的纹理坐标的(0,1)
    vTexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
    gl_Position = aPosition;
}