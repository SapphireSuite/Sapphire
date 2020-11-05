// Copyright 2020 Sapphire development team. All Rights Reserved.

#version 450

#extension GL_ARB_separate_shader_objects : enable

// In.
layout (triangles) in; // Current rendered triangle.


// Out.
layout(location = 0) out vec4 outPosition;

layout (triangle_strip, max_vertices=18) out; // triangles generated on each faces (6).


// Code.
void ComputeRightFace();
void ComputeLeftFace();
void ComputeUpFace();
void ComputeDownFace();
void ComputeFrontFace();
void ComputeBackFace();

void main()
{
	ComputeRightFace();
	ComputeLeftFace();
	ComputeUpFace();
	ComputeDownFace();
	ComputeFrontFace();
	ComputeBackFace();
}


void ComputeRightFace()
{
	/**
	*	Rotation matrix for right cubemap face.
	*	Formula: https://en.wikipedia.org/wiki/Rotation_matrix#In_three_dimensions
	*
	*	Right-face rotation: Pi/2 on Y axis.
	*/
	const mat4 rightMat = mat4(
		0.0, 0.0, 1.0, 0.0,				/* cos(Pi/2), 0.0, sin(Pi/2), 0.0 */
		0.0, 1.0, 0.0, 0.0,
		-1.0, 0.0, 0.0, 0.0,			/* -sin(Pi/2), 0.0, cos(Pi/2), 0.0*/
		0.0, 0.0, 0.0, 1.0
	);


	// Current rendered face (built-in variable).
	gl_Layer = 0;

	// for each triangle vertex
	for(int i = 0; i < 3; ++i)
	{
		outPosition = gl_in[i].gl_Position;
		gl_Position = rightMat * outPosition;

		EmitVertex();
	}

	EndPrimitive();
}

void ComputeLeftFace()
{
	/**
	*	Rotation matrix for left cubemap face.
	*	Formula: https://en.wikipedia.org/wiki/Rotation_matrix#In_three_dimensions
	*
	*	Left-face rotation: Pi/2 on Y axis.
	*/
	const mat4 leftMat = mat4(
		0.0, 0.0, -1.0, 0.0,			/* cos(-Pi/2), 0.0, sin(-Pi/2), 0.0 */
		0.0, 1.0, 0.0, 0.0,
		1.0, 0.0, 0.0, 0.0,				/* -sin(-Pi/2), 0.0, cos(-Pi/2), 0.0*/
		0.0, 0.0, 0.0, 1.0
	);


	// Current rendered face (built-in variable).
	gl_Layer = 1;

	// for each triangle vertex
	for(int i = 0; i < 3; ++i)
	{
		outPosition = gl_in[i].gl_Position;
		gl_Position = leftMat * outPosition;

		EmitVertex();
	}

	EndPrimitive();
}

void ComputeUpFace()
{
	/**
	*	Rotation matrix for up cubemap face.
	*	Formula: https://en.wikipedia.org/wiki/Rotation_matrix#In_three_dimensions
	*
	*	Up-face rotation: Pi/2 on Y axis.
	*/
	const mat4 upMat = mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 0.0, -1.0, 0.0,			/* 0.0, cos(Pi/2), -sin(Pi/2), 0.0 */
		0.0, 1.0, 0.0, 0.0,				/* 0.0, sin(Pi/2), cos(Pi/2), 0.0 */
		0.0, 0.0, 0.0, 1.0
	);


	// Current rendered face (built-in variable).
	gl_Layer = 2;

	// for each triangle vertex
	for(int i = 0; i < 3; ++i)
	{
		outPosition = gl_in[i].gl_Position;
		gl_Position = upMat * outPosition;

		EmitVertex();
	}

	EndPrimitive();
}

void ComputeDownFace()
{
	/**
	*	Rotation matrix for down cubemap face.
	*	Formula: https://en.wikipedia.org/wiki/Rotation_matrix#In_three_dimensions
	*
	*	Down-face rotation: Pi/2 on Y axis.
	*/
	const mat4 downMat = mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,				/* 0.0, cos(-Pi/2), -sin(-Pi/2), 0.0 */
		0.0, -1.0, 0.0, 0.0,			/* 0.0, sin(-Pi/2), cos(-Pi/2), 0.0 */
		0.0, 0.0, 0.0, 1.0
	);


	// Current rendered face (built-in variable).
	gl_Layer = 3;

	// for each triangle vertex
	for(int i = 0; i < 3; ++i)
	{
		outPosition = gl_in[i].gl_Position;
		gl_Position = downMat * outPosition;

		EmitVertex();
	}

	EndPrimitive();
}

void ComputeFrontFace()
{
	/*
	*	Rotation matrix for front cubemap face is identity.
	*/

	// Current rendered face (built-in variable).
	gl_Layer = 4;

	// for each triangle vertex
	for(int i = 0; i < 3; ++i)
	{
		outPosition = gl_in[i].gl_Position;
		gl_Position = outPosition;

		EmitVertex();
	}

	EndPrimitive();
}

void ComputeBackFace()
{
	/**
	*	Rotation matrix for back cubemap face.
	*	Formula: https://en.wikipedia.org/wiki/Rotation_matrix#In_three_dimensions
	*
	*	Back-face rotation: Pi/2 on Y axis.
	*/
	const mat4 backMat = mat4(
		-1.0, 0.0, 0.0, 0.0,			/* cos(Pi), 0.0, sin(Pi), 0.0 */
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, -1.0, 0.0,			/* -sin(Pi), 0.0, cos(Pi), 0.0*/
		0.0, 0.0, 0.0, 1.0
	);


	// Current rendered face (built-in variable).
	gl_Layer = 5;

	// for each triangle vertex
	for(int i = 0; i < 3; ++i)
	{
		outPosition = gl_in[i].gl_Position;
		gl_Position = backMat * outPosition;

		EmitVertex();
	}

	EndPrimitive();
}