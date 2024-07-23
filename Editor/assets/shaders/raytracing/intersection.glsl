


HitInfo RayTriangle(Ray ray, Triangle tri)
{
    vec3 edgeAB = tri.posB - tri.posA;
    vec3 edgeAC = tri.posC - tri.posA;
	vec3 normalVector = cross(edgeAB, edgeAC);
    vec3 ao = ray.origin - tri.posA;
    vec3 dao = cross(ao, ray.dir);

    float determinant = -dot(ray.dir, normalVector);
    float invDet = 1 / determinant;

    // Calculate distance to triangle & barycentric coordinates of intersection point
    float dst = dot(ao, normalVector) * invDet;
    float u = dot(edgeAC, dao) * invDet;
    float v = -dot(edgeAB, dao) * invDet;
    float w = 1 - u - v;

    // Initialize hit info
    HitInfo hitInfo;
    hitInfo.hit = abs(determinant) >= EPSILON && dst >= 0 && u >= 0 && v >= 0 && w >= 0;
	// hitInfo.hit = determinant >= EPSILON && dst >= 0 && u >= 0 && v >= 0 && w >= 0;

	// hitInfo.hit = determinant >= EPSILON && dst >= 0 && u >= 0 && v >= 0 && w >= 0;  // Note the use of fabs
    hitInfo.position = ray.origin + ray.dir * dst;
    hitInfo.normal = normalize(normalVector);
    hitInfo.distance = dst;
	// greaterThanEqual(hitInfo.distance, 0.0);
    // hitInfo.material = tri.material;

	// hitInfo.material.albedo = (hitInfo.normal * 0.5) + 0.5;
	hitInfo.material.albedo = vec3(0.8471, 0.8471, 0.8471);
	// hitInfo.material.albedo = 0.5 * (normalVector + vec3(1.0));
	// hitInfo.material.albedo = vec3(u, v, w);
    return hitInfo;
}

// HitInfo RayTriangle(Ray ray, Triangle tri)
// {
// 	vec3 v0 = tri.posA;
// 	vec3 v1 = tri.posB;
// 	vec3 v2 = tri.posC;

// 	vec3 e0 = v1 - v0;
// 	vec3 e1 = v2 - v0;
// 	vec3 pv = cross(ray.dir, e1);
// 	float det = dot(e0, pv);

// 	vec3 tv = ray.origin - v0;
// 	vec3 qv = cross(tv, e0);

// 	float u = dot(tv, pv) / det;
// 	float v = dot(ray.dir, qv) / det;
// 	float t = dot(e1, qv) / det;

// 	float w = 1.0 - u - v;

// 	if (u >= 0.0 && v >= 0.0 && w >= 0.0 && t >= 0.0)
// 	{
// 		HitInfo hitinfo;
// 		hitinfo.hit = true;
// 		hitinfo.position = ray.origin + ray.dir * t;
// 		// hitinfo.normal = normalize(tri.normal);
// 		// hitinfo.normal = normalize( )
// 		// I dont have tri.normal
// 		hitinfo.normal = normalize(cross(e0, e1));
// 		hitinfo.distance = t;
// 		hitinfo.material.albedo = vec3(0.698, 0.698, 0.698);
// 		return hitinfo;
// 	}
// 	else
// 	{
// 		HitInfo hitinfo;
// 		hitinfo.hit = false;
// 		hitinfo.distance = INFINITY;
// 		return hitinfo;
// 	}
// }



// float RayBoundingBoxDst(Ray ray, vec3 boxMin, vec3 boxMax)
// {
// 	vec3 tMin = (boxMin - ray.origin) * ray.invDir;
// 	vec3 tMax = (boxMax - ray.origin) * ray.invDir;
// 	vec3 t1 = min(tMin, tMax);
// 	vec3 t2 = max(tMin, tMax);
// 	float tNear = max(max(t1.x, t1.y), t1.z);
// 	float tFar = min(min(t2.x, t2.y), t2.z);

// 	bool hit = tFar >= tNear && tFar > 0;
// 	float dst = hit ? tNear > 0 ? tNear : 0 : INFINITY;
// 	return dst;
// };

float RayBoundingBoxDst(Ray ray, vec3 boxMin, vec3 boxMax)
{
	vec3 tMin = (boxMin - ray.origin) * ray.invDir;
	vec3 tMax = (boxMax - ray.origin) * ray.invDir;
	vec3 t1 = min(tMin, tMax);
	vec3 t2 = max(tMin, tMax);
	float tNear = max(max(t1.x, t1.y), t1.z);
	float tFar = min(min(t2.x, t2.y), t2.z);

	bool hit = tFar >= tNear && tFar > 0;
	return hit ? tNear : INFINITY;
};


HitInfo RaySphere(Ray ray, Sphere sphere, HitInfo hitInfo)
{
	vec3 offsetRayOrigin = ray.origin - sphere.position;

	float a = dot(ray.dir, ray.dir);
	float b = 2.0 * dot(ray.dir, offsetRayOrigin);
	float c = dot(offsetRayOrigin, offsetRayOrigin) - sphere.radius * sphere.radius;

	float discriminant = b * b - 4.0 * a * c;

	if (discriminant > 0.0)
	{
		float dist = (-b - sqrt(discriminant)) / (2.0 * a);
		if (dist > 0.0 && dist < hitInfo.distance)
		{
			hitInfo.hit = true;
			hitInfo.position = ray.origin + ray.dir * dist;
			hitInfo.normal = normalize(hitInfo.position - sphere.position);
			hitInfo.distance = dist;
			hitInfo.material = sphere.material;
			// hitInfo.material.albedo = normalize(hitInfo.position - sphere.position);
		}
	}
	return hitInfo;
}