#ifndef ___VERTEX_STRUCTS_H___
#define ___VERTEX_STRUCTS_H___

struct VertexData
{
    vec3 m_pos;
    vec4 m_color;

    VertexData() {}

    VertexData(vec3 pos, vec4 col)
    {
        m_pos    = pos;
        m_color  = col;

    }
};

struct pnVertexData
{
    vec3 m_pos;
    vec3 m_normal;

    pnVertexData() {}

    pnVertexData(vec3 pos, vec3 norm)
    {
        m_pos    = pos;
        m_normal  = norm;
    }
};

struct pntVertexData
{
    vec3 m_pos;
    vec3 m_normal;
	vec2 m_text;

    pntVertexData() {}

    pntVertexData(vec3 pos, vec3 norm, vec2 text)
    {
        m_pos    = pos;
        m_normal  = norm;
		m_text = text;
    }
};



#endif /* ___VERTEX_STRUCTS_H___ */