#pragma once

class CapsuleCollider : public Collider
{
public:
    CapsuleCollider(float radius = 1.0f, float height = 2.0f,
        UINT stackCount = 9, UINT sliceCount = 16);

    //virtual void Edit() override;

    bool IsRayCollision(const Ray& ray, RayHit* hitInfo) override;
    bool IsBoxCollision(BoxCollider* collider) override;
    bool IsSphereCollision(SphereCollider* collider) override;
    bool IsCapsuleCollision(CapsuleCollider* collider) override;

    float Radius() {
        return radius * max(GetGlobalScale().x,
            max(GetGlobalScale().y, GetGlobalScale().z));
    }

    float Height() { return height * GetGlobalScale().y; }

private:
    virtual void MakeMesh() override;
    void UpdateMesh();

private:
    float radius, height;
    UINT stackCount, sliceCount;
};