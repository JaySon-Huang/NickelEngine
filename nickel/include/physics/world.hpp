#pragma once

#include "pch.hpp"
#include "core/assert.hpp"
#include "physics/manifold_solver.hpp"
#include "physics/physic_solver.hpp"
#include "physics/circle_shape.hpp"
#include "physics/capsule_shape.hpp"
#include "physics/polygon_shape.hpp"
#include "physics/obb_shape.hpp"
#include "renderer/renderer2d.hpp"


namespace nickel {

namespace physics {

class World final {
public:
    using ForceGenerator = std::function<void(Body&)>;

    void Step(Real interval, gecs::querier<gecs::mut<Body>, CollideShape>, gecs::resource<gecs::mut<Renderer2D>> renderer);
    std::vector<ForceGenerator> forceGenerators;

    Real MaxSpeed() const { return physicSolver_.MaxSpeed(); }
    void SetMaxSpeed(Real s) { return physicSolver_.SetMaxSpeed(s); }

private:
    PhysicSolver physicSolver_;
    ManifoldSolver manifoldSolver_;

    void collide(gecs::querier<gecs::mut<Body>, CollideShape> bodies, Renderer2D&);
    void dealContact(const Vec2& mtv,  const Vec2& tangent, Body& b1, Body& b2, bool = true);
};

void PhysicsInit(gecs::commands cmds);

void PhysicsUpdate(gecs::resource<gecs::mut<World>> world,
                   gecs::querier<gecs::mut<Body>, CollideShape> querier,
                   gecs::resource<gecs::mut<Renderer2D>> renderer);

template <typename T>
T shape_cast(const Shape& s) {
    using type = std::decay_t<T>;
    switch (s.GetType()) {
        case Shape::Type::Circle:
            Assert((std::is_same_v<type, CircleShape>),
                   "cast shape to circle failed");
            break;
        case Shape::Type::OBB:
            Assert((std::is_same_v<type, OBBShape>),
                   "cast shape to circle failed");
            break;
        case Shape::Type::Polygon:
            Assert((std::is_same_v<type, PolygonShape>),
                   "cast shape to circle failed");
            break;
        case Shape::Type::Capsule:
            Assert((std::is_same_v<type, CapsuleShape>),
                   "cast shape to circle failed");
            break;
        default:
            Assert(false, "cast shape to no physics shape is invalid");
    }

    return static_cast<T>(s);
}

}  // namespace physics

}  // namespace nickel