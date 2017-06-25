
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;
varying vec3 face_normal;

uniform sampler2D MAP_SILVER;
uniform sampler2D MAP_WHITE_STEEL;
uniform sampler2D MAP_BRUSHED_STAINLESS;

void main()
{
	vec4 color;
	
	if((1 - abs(face_normal.y)) > 0.2) color = texture2D(MAP_SILVER, v_uv);
	else if(face_normal.y > 0) color = texture2D(MAP_BRUSHED_STAINLESS, v_uv);
	else color = texture2D(MAP_WHITE_STEEL, v_uv);
	
	gl_FragColor = color;

}
