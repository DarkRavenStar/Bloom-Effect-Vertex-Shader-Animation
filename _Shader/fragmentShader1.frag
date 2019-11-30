precision mediump float;
varying vec4 fColor;
varying vec2 fTexCoord;

uniform sampler2D sampler2d;
uniform int uBlurDirection;
uniform float uTextureW;
uniform float uTextureH;

float gaussianFunction(float x)
{
	float variance = 0.15; //x should be 0-1.0 with this variance

	float alpha = -(x*x / (2.0*variance));
	return exp(alpha);
}

float gaussianFunction2D(float x, float y)
{
	float variance = 0.25; //x and y should be 0-1.0 with this variance

	float alpha = -( (x*x+y*y) / (2.0*variance));
	return exp(alpha);
}

void main()
{
	float textureW = uTextureW;
	float textureH = uTextureH;

	float radiusSize = 100.0;
	float rSkip = 4.0;
	float totalWeight = 0.0;
	//float radiusSqr = radiusSize * radiusSize;

	vec4 accumulatedColor;

	if(uBlurDirection == 0) //vertical blur
	{
		float u = fTexCoord.x;
		float y;
		
		for(y=-radiusSize; y<=radiusSize; y+=rSkip)
		{
			float v = fTexCoord.y + y/textureH;
			
			if(v>=0.0 && v<=1.0)
			{
				float weight = gaussianFunction(y/radiusSize);
				accumulatedColor += texture2D(sampler2d, vec2(u,v)) * weight;
				totalWeight += weight;
			}
		}
		
		gl_FragColor = accumulatedColor / totalWeight;
	}
	else if(uBlurDirection == 1) //horizontal blur
	{
		float v = fTexCoord.y;
		float x;
		
		for(x=-radiusSize; x<=radiusSize; x+=rSkip)
		{
			float u = fTexCoord.x + x/textureW;
			
			if(u>=0.0 && u<=1.0)
			{
				float weight = gaussianFunction(x/radiusSize);
				accumulatedColor += texture2D(sampler2d, vec2(u,v)) * weight;
				totalWeight += weight;
			}
		}
		
		gl_FragColor = accumulatedColor / totalWeight;
	}
	else if(uBlurDirection == 2) //High Pass Filter
	{
		vec4 testColor = texture2D(sampler2d, fTexCoord);
		float averageColor = (testColor.r + testColor.g + testColor.b) / 3.0;
		if(averageColor > 0.6)
		{
			gl_FragColor = testColor;
		}
		else
		{
			gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
		}
	}
	else //no blur
	{
		gl_FragColor = texture2D(sampler2d, fTexCoord);
	}

}
