#include "mesh.h"
#include "../utility.h"

#include <string.h>
#include <math.h>

object_t *mesh_create(size_t vertex_buffer_size, size_t index_buffer_size, material_t *material)
{
	mesh *m = malloc(sizeof *m);
	if (m == NULL)
		ERROR(ERROR_ALLOC_FAIL, ERROR_MESH, ERROR_NULL);

	*m = (mesh){
		.hit = &mesh_hit,
		.material = material,
        .free_data = &mesh_free,

		.vertex_buffer_size = vertex_buffer_size,
		.index_buffer_size = index_buffer_size
	};

    m->vertex_buffer = malloc(vertex_buffer_size * sizeof *m->vertex_buffer);
    if (m == NULL)
        ERROR(ERROR_ALLOC_FAIL, ERROR_VB, ERROR_NULL);

    m->index_buffer = malloc(index_buffer_size * sizeof * m->index_buffer);
    if (m == NULL)
        ERROR(ERROR_ALLOC_FAIL, ERROR_IB, ERROR_NULL);

	return (object_t*)m;
}

bool mesh_hit(object_t *m, ray_t *ray, double tmin, double tmax, hit_record *record)
{
    mesh *m_ = (mesh *)m;
    double t_nearest = tmax;
    bool hit = false;
	
    for (size_t i = 0; i < m_->index_buffer_size; i += 3)
    {
        vec3 v1 = m_->vertex_buffer[m_->index_buffer[i]];
        vec3 v2 = m_->vertex_buffer[m_->index_buffer[i + 1]];
        vec3 v3 = m_->vertex_buffer[m_->index_buffer[i + 2]];

        /* Calculate normal */
        vec3 vd2, vd3, nrm;
        vec3_copy(&vd2, &v2);
        vec3_copy(&vd3, &v3);
        vec3_sub(&vd2, &vd2, &v1);
        vec3_sub(&vd3, &vd3, &v1);

        vec3_crs(&nrm, &vd2, &vd3);
        vec3_nrm(&nrm, &nrm);


        /* Plane intersection */
        double denom;
        double nom;
        double t;
        vec3 vtmp;

        vec3_dot(&denom, &ray->dir, &nrm);

        /* Ray is parallell */
        if (denom == 0)
            continue;

        /* Calculate intersection */
        vec3_copy(&vtmp, &v1);
        vec3_sub(&vtmp, &vtmp, &ray->org);
        vec3_dot(&nom, &vtmp, &nrm);

        t = nom / denom;

        if (t < tmin || t > t_nearest)
            continue;

        /* Projection */
        vec3 rv;
        ray_at(&rv, ray, t);
        double 
            x1 = v1.x,
            x2 = v2.x,
            x3 = v3.x,
            rx = rv.x,

            y1 = v1.y,
            y2 = v2.y,
            y3 = v3.y,
            ry = rv.y;

        double det = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
        if (det < EPSILON && det > -EPSILON)
        {
            y1 = v1.z;
            y2 = v2.z;
            y3 = v3.z;
            ry = rv.z;
        }

        det = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
        if (det < EPSILON && det > -EPSILON)
        {
            x1 = v1.y;
            x2 = v2.y;
            x3 = v3.y;
            rx = rv.y;
        }


        /* Re-orient */
        x2 -= x1;
        x3 -= x1;
        rx -= x1;

        y2 -= y1;
        y3 -= y1;
        ry -= y1;

        if (x2 * y3 - x3 * y2 < 0)
        {
            double tmp = x2;
            x2 = x3;
            x3 = tmp;

            tmp = y2;
            y2 = y3;
            y3 = tmp;
        }


        /* Check if ray point is in triangle */
        if (
            x2 * ry - y2 * rx < 0 ||
            (x3 - x2) * (ry - y2) - (y3 - y2) * (rx - x2) < 0 ||
            -x3 * (ry - y3) - -y3 * (rx - x3) < 0
            )
            continue;

        /* Hit! */
        t_nearest = t;
        record->t = t;
        record->mat = m_->material;
        ray_at(&record->point, ray, t);
        hrec_set_normal(record, &nrm, ray);

        hit = true;
    }

    return hit;
}

void mesh_load_vertex_buffer(mesh *m, vec3 *buffer)
{
	memcpy(m->vertex_buffer, buffer, m->vertex_buffer_size * sizeof *m->vertex_buffer);
    /* MOVE BY POS */
}

void mesh_load_index_buffer(mesh *m, uint32_t *buffer)
{
	memcpy(m->index_buffer, buffer, m->index_buffer_size * sizeof *buffer);
}

void mesh_rotate(mesh *m, double u, double v, double w)
{
    double a, b;

    for (size_t i = 0; i < m->vertex_buffer_size; i++)
    {
        vec3 *vert = &m->vertex_buffer[i];

        a = vert->y;
        b = vert->z;

        vert->y = cos(u) * a - sin(u) * b;
        vert->z = sin(u) * a + cos(u) * b;

        a = vert->x;
        b = vert->z;

        vert->x = cos(v) * a - sin(v) * b;
        vert->z = sin(v) * a + cos(v) * b;

        a = vert->x;
        b = vert->y;

        vert->x = cos(w) * a - sin(w) * b;
        vert->y = sin(w) * a + cos(w) * b;
    }
}

void mesh_translate(mesh *m, vec3 pos)
{
    for (size_t i = 0; i < m->vertex_buffer_size; i++)
    {
        vec3 *vert = &m->vertex_buffer[i];

        vec3_add(vert, vert, &pos);
    }
}

void mesh_scale(mesh *m, double sx, double sy, double sz)
{
    for (size_t i = 0; i < m->vertex_buffer_size; i++)
    {
        vec3 *vert = &m->vertex_buffer[i];

        vert->x *= sx;
        vert->y *= sy;
        vert->z *= sz;
    }
}

void mesh_free(object_t *m)
{
    free(((mesh *)m)->vertex_buffer);
    free(((mesh *)m)->index_buffer);
}