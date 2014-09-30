#version 330
#extension GL_EXT_geometry_shader4 : enable

layout(points) in;
 
layout(triangle_strip, max_vertices = 4) out;

in vec4 color[];
out vec4 gColor;

uniform mat4 myRotationMatrix;

const float delta=0.025f;
void main()
{
  int i;
  for(i = 0; i < gl_in.length (); ++i)
  	{
	    gl_Position = gl_in[i].gl_Position + myRotationMatrix*vec4(delta,-delta,0.0,0.0);
	   gColor = color[i];
       EmitVertex(); 
	   gl_Position = gl_in[i].gl_Position + myRotationMatrix*vec4(-delta,-delta,0.0,0.0);
	   gColor = color[i];
       EmitVertex();
	   gl_Position = gl_in[i].gl_Position + myRotationMatrix*vec4(delta,delta,0.0,0.0);
	   gColor = color[i];
       EmitVertex();
	   gl_Position = gl_in[i].gl_Position + myRotationMatrix*vec4(-delta,delta,0.0,0.0);
	   gColor = color[i];
       EmitVertex();	 	   
	}
   EndPrimitive();
}

/*void main()
{
  int i;
  for(i = 0; i < gl_in.length (); ++i)
  	{
	   gl_Position = gl_in[i].gl_Position + vec4(delta,-delta,0.0,0.0);
	   gColor = color[i];
       EmitVertex(); 
	   gl_Position = gl_in[i].gl_Position + vec4(-delta,-delta,0.0,0.0);
	   gColor = color[i];
       EmitVertex();
	   gl_Position = gl_in[i].gl_Position + vec4(delta,delta,0.0,0.0);
	   gColor = color[i];
       EmitVertex();
	   gl_Position = gl_in[i].gl_Position + vec4(-delta,delta,0.0,0.0);
	   gColor = color[i];
       EmitVertex();	   
	}
   EndPrimitive();
}*/