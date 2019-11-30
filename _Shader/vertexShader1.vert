attribute vec4 vPosition;
attribute vec4 vColor;
attribute vec2 vTexCoord;

varying vec2 fTexCoord;
varying vec4 fColor;

uniform mat4 uMvpMatrix;
uniform int vertexAnimation;
uniform float Factor1;

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void main()
{
	fColor = vColor;
	fTexCoord = vTexCoord;
	
	if(vertexAnimation == 0)
	{
		mat4 Matrix = uMvpMatrix;
		float angle = cos(Factor1 * cos(Factor1 * 1.0) * 10.0 * vPosition.x) * 10.0 * vPosition.y;
		Matrix = rotationMatrix(vec3(0.0, 1.0, 0.0), angle);
		
		float speed = 100.0 * cos(Factor1 * 1.0);
		float x = vPosition.x * (speed + 10.0);
		float z = vPosition.z * (speed + 10.0);
		float y = vPosition.y;
		float w = vPosition.w;
		
		vec4 finalVPosition = vec4(x, y, z, w);
		gl_Position = uMvpMatrix * Matrix * finalVPosition;
	}
	else if(vertexAnimation == 1)
	{
		float angle = cos(Factor1 * 1.0) * 10.0;
		mat4 Matrix = rotationMatrix(vec3(1.0, 0.0, 0.0), angle);
		float speed = 100.0 * cos(Factor1 * 1.0);
		float x = vPosition.x * (speed + 10.0);
		float z = vPosition.z * (speed + 10.0);
		float y = vPosition.y;
		float w = vPosition.w;
		
		vec4 finalVPosition = vec4(x, y, z, w);
		gl_Position = uMvpMatrix * Matrix * finalVPosition;
	}
	else if(vertexAnimation == 2)
	{
		mat4 Matrix = uMvpMatrix;
		float angle = cos(Factor1 * cos(Factor1 * 1.0) * 10.0 * vPosition.x) * 10.0 * vPosition.y;
		Matrix = rotationMatrix(vec3(0.0, 1.0, 0.0), angle);
		
		angle = cos(Factor1 * 1.0) * 10.0;
		float speed = 100.0 * cos(Factor1 * 1.0);
		float x = vPosition.x * (speed + 10.0);
		float z = vPosition.z * (speed + 10.0);
		float y = vPosition.y;
		float w = vPosition.w;
		
		vec4 finalVPosition = vec4(x, y, z, w);
		Matrix += rotationMatrix(vec3(1.0, 0.0, 0.0), angle);
		gl_Position = uMvpMatrix * Matrix * finalVPosition;
	}
	else
	{
		gl_Position = uMvpMatrix * vPosition;
	}
}
