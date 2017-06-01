
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;
varying vec3 face_normal;

uniform sampler2D BEADBOARD;
uniform sampler2D FABRIC_LINEN;
uniform sampler2D LEATHER_BLUE;

void main()
{

	//vec4 color = texture2D(BEADBOARD, v_uv);
	vec4 color;
	//v_uv = v_uv;
	if((1 - abs(face_normal.y)) > 0.2) color = texture2D(BEADBOARD, v_uv);
	else if(face_normal.y > 0) color = texture2D(LEATHER_BLUE, v_uv);
	else color = texture2D(FABRIC_LINEN, v_uv);
	
	gl_FragColor = color;

}
