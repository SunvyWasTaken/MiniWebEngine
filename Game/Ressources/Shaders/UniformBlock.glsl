layout (std140, binding = 0) uniform Camera
{
	mat4 projection;
	mat4 view;
	mat4 relativeView;
};