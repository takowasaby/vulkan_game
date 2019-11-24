#include <chrono>
#include <thread>
#include <algorithm>

#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/gtc/matrix_transform.hpp>

#include <Wasabi/window/glfw/window.h>
#include <Wasabi/graphic/vulkan/graphicEngine.h>

#include <Wasabi/graphic/vulkan/vertex.h>
#include <Wasabi/graphic/vulkan/uniformBufferObject.h>

#include <Wasabi/input/mouse.h>

using namespace wsb;

constexpr float speed = 1.0f;

enum Direction {
	left	= 0x00000001,
	right	= 0x00000002,
	top		= 0x00000004,
	bottom	= 0x00000008,
};

class FpsKeeper {
public:
	static long long calcOneFrameDuration(int fps) {
		return std::chrono::nanoseconds::period::den / static_cast<long long>(fps);
	}
	FpsKeeper(int fps) :
		_oneFrameDuration(calcOneFrameDuration(fps))
	{}
	void frameWait() {
		auto currentFrameTime = std::chrono::high_resolution_clock::now();
		if (_prevFrameTime.has_value()) {
			auto duration = currentFrameTime - _prevFrameTime.value();
			auto countDiff = _oneFrameDuration < duration.count() ? 0ll : _oneFrameDuration - duration.count();
			std::this_thread::sleep_for<long long, std::milli>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::nanoseconds(countDiff)));
		}
		_prevFrameTime = currentFrameTime;
	}
private:
	long long _oneFrameDuration;
	std::optional<std::chrono::high_resolution_clock::time_point> _prevFrameTime;
};

class PointCollider {
public:
	PointCollider(float x, float y) 
		: _x(x)
		, _y(y)
	{}
	float _x, _y;
};

class SquareCillider {
public:
	SquareCillider(float x1, float y1, float x2, float y2)
		: _x1(x1)
		, _y1(y1)
		, _x2(x2)
		, _y2(y2)
	{}
	float _x1, _y1, _x2, _y2;
};

class CircleCillider {
public:
	CircleCillider(float cx, float cy, float radius)
		: _cx(cx)
		, _cy(cy)
		, _radius(radius)
	{}
	float _cx, _cy, _radius;
};

class CollisionDetectioner {
public:
	static bool isColliding(PointCollider point, CircleCillider circle) {
		float xDiff = fabsf(point._x - circle._cx);
		float yDiff = fabsf(point._y - circle._cy);
		return xDiff * xDiff + yDiff * yDiff < circle._radius * circle._radius;
	}

	static int32_t CollidingDirection(SquareCillider square, CircleCillider circle) {
		if (square._x1 < circle._cx && circle._cx < square._x2) {
			bool touch = circle._cy + circle._radius >= square._y1 && circle._cy - circle._radius <= square._y2;
			if (touch) {
				float squareCY = (square._y1 + square._y2) / 2.0f;
				if (circle._cy >= squareCY) return Direction::top;
				else return Direction::bottom;
			}
		}
		else if (square._y1 < circle._cy && circle._cy < square._y2) {
			bool touch = circle._cx + circle._radius >= square._x1 && circle._cx - circle._radius <= square._x2;
			if (touch) {
				float squareCX = (square._x1 + square._x2) / 2.0f;
				if (circle._cx >= squareCX) return Direction::right;
				else return Direction::left;
			}
		}
		else {
			if (circle._cx < square._x1 && circle._cy < square._y1) {
				if (isColliding(PointCollider(square._x1, square._y1), circle)) {
					return Direction::left & Direction::top;
				}
			}
			else if (circle._cx < square._x1 && circle._cy < square._y2) {
				if (isColliding(PointCollider(square._x1, square._y2), circle)) {
					return Direction::left & Direction::bottom;
				}
			}
			else if (circle._cx < square._x2 && circle._cy < square._y1) {
				if (isColliding(PointCollider(square._x2, square._y1), circle)) {
					return Direction::right & Direction::top;
				}
			}
			else if (circle._cx < square._x2 && circle._cy < square._y2) {
				if (isColliding(PointCollider(square._x2, square._y2), circle)) {
					return Direction::right & Direction::bottom;
				}
			}
		}
		return 0;
	}
};

class IShape {
public:
	virtual ~IShape() = default;
	virtual std::vector<graphic::vulkan::Vertex> vertices() const = 0;
	virtual std::vector<uint16_t> indices(uint16_t min = 0) const = 0;
};

class Square : public IShape {
public:
	Square(float x1, float y1, float x2, float y2) 
		: _x1(x1)
		, _y1(y1)
		, _x2(x2)
		, _y2(y2)
		, _isInCollision(false)
		, _isActive(true)
	{}
	Square(float x1, float y1, float x2, float y2, bool isInCollision, bool isActive)
		: _x1(x1)
		, _y1(y1)
		, _x2(x2)
		, _y2(y2)
		, _isInCollision(isInCollision)
		, _isActive(isActive)
	{}
	Square move(float x, float y) {
		return Square(_x1 + x, _y1 + y, _x2 + x, _y2 + y, _isInCollision, _isActive);
	}
	float getLeft() const {
		return _x1;
	}
	float getRight() const {
		return _x2;
	}
	Square setLeft(float left) {
		float diff = left - _x1;
		return Square(
			_x1 + diff,
			_y1,
			_x2 + diff,
			_y2,
			_isInCollision, 
			_isActive
		);
	}
	Square setRight(float right) {
		float diff = right - _x2;
		return Square(
			_x1 + diff,
			_y1,
			_x2 + diff,
			_y2,
			_isInCollision, 
			_isActive
		);
	}

	std::vector<graphic::vulkan::Vertex> vertices() const override {
		std::vector<graphic::vulkan::Vertex> vertices;
		if (_isActive) {
			vertices = {
				{{_x1, _y1}, {1.0f, 0.0f, 0.0f, 1.0f}},
				{{_x2, _y1}, {0.0f, 1.0f, 0.0f, 1.0f}},
				{{_x2, _y2}, {0.0f, 0.0f, 1.0f, 1.0f}},
				{{_x1, _y2}, {1.0f, 1.0f, 1.0f, 1.0f}}
			};
		}
		else {
			vertices = {
				{{_x1, _y1}, {1.0f, 0.0f, 0.0f, 0.0f}},
				{{_x2, _y1}, {0.0f, 1.0f, 0.0f, 0.0f}},
				{{_x2, _y2}, {0.0f, 0.0f, 1.0f, 0.0f}},
				{{_x1, _y2}, {1.0f, 1.0f, 1.0f, 0.0f}}
			};
		}
		return vertices;
	}
	std::vector<uint16_t> indices(uint16_t min = 0) const override {
		std::vector<uint16_t> indices = {
			0, 1, 2, 2, 3, 0, 2, 1, 0, 0, 3, 2
		};
		for (auto itr = indices.begin(); itr != indices.end(); itr++) {
			*itr += min;
		}
		return indices;
	}
	std::optional<SquareCillider> collider() const {
		std::optional<SquareCillider> collider;
		if (_isActive) collider = SquareCillider(_x1, _y1, _x2, _y2);
		return collider;
	}
	bool isInCollision() const {
		return _isInCollision;
	}
	Square intoCollision() {
		return Square(
			_x1,
			_y1,
			_x2,
			_y2,
			true, 
			_isActive
		);
	}
	Square outFromCollision() {
		return Square(
			_x1,
			_y1,
			_x2,
			_y2,
			false, 
			_isActive
		);
	}
	Square destroy() {
		return Square(
			_x1,
			_y1,
			_x2,
			_y2,
			_isInCollision,
			false
		);
	}
private:
	float _x1, _y1, _x2, _y2;
	bool _isInCollision;
	bool _isActive;
};

class Circle : public IShape {
public:
	Circle(float cx, float cy, float radius)
		: _cx(cx)
		, _cy(cy)
		, _radius(radius)
	{}
	Circle move(float x, float y) {
		return Circle(_cx + x, _cy + y, _radius);
	}
	std::vector<graphic::vulkan::Vertex> vertices() const override {
		std::vector<graphic::vulkan::Vertex> vertices;

		vertices.push_back({ {_cx, _cy}, {1.0f, 1.0f, 1.0f, 1.0f} });
		for (int i = 0; i < 8; i++) {
			float x = _cx + _radius * cosf(0.25 * i * M_PI);
			float y = _cy + _radius * sinf(0.25 * i * M_PI);
			if (abs(x) < 0.001f) x = 0.0f;
			if (abs(y) < 0.001f) y = 0.0f;
			vertices.push_back({ {x, y}, {i / 9.0f, i / 9.0f, i / 9.0f, 1.0f} });
		}
		return vertices;
	}
	std::vector<uint16_t> indices(uint16_t min = 0) const override {
		std::vector<uint16_t> indices = {
			0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6, 0, 6, 7, 0, 7, 8, 0, 8, 1,
			0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 5, 4, 0, 6, 5, 0, 7, 6, 0, 8, 7, 0, 1 ,8
		};
		for (auto itr = indices.begin(); itr != indices.end(); itr++) {
			*itr += min;
		}
		return indices;
	}
	CircleCillider collider() const {
		return CircleCillider(_cx, _cy, _radius);
	}
private:
	float _cx, _cy, _radius;
};

class Player {
public:
	Player()
		: _body(std::make_shared<Square>(-0.25f, 0.95f, 0.25f, 1.0f))
	{}
	void update(const wsb::window::glfw::Window& window, const wsb::input::Mouse& mouse) {
		*_body = _body->move(-(mouse.getMousePosX() / window.getWindowWidth() - 0.5) / 10, 0);
		if (_body->getLeft() < -1.0f) {
			*_body = _body->setLeft(-1.0f);
		}
		if (_body->getRight() > 1.0f) {
			*_body = _body->setRight(1.0f);
		}
	}
	static Player createPlayer(std::vector<std::shared_ptr<IShape>>& renderShapes, std::vector<std::shared_ptr<Square>>& squares) {
		auto player = Player();
		renderShapes.push_back(player._body);
		squares.push_back(player._body);
		return player;
	}
private:
	std::shared_ptr<Square> _body;
};

class Block {
public:
	Block(float x, float y)
		: _body(std::make_shared<Square>(x - 0.1f, y - 0.04f, x + 0.1f, y + 0.04f))
	{}
	void update() {

	}
	static Block createBlock(float x, float y, std::vector<std::shared_ptr<IShape>>& renderShapes, std::vector<std::shared_ptr<Square>>& blocks) {
		auto block = Block(x, y);
		renderShapes.push_back(block._body);
		blocks.push_back(block._body);
		return block;
	}
private:
	std::shared_ptr<Square> _body;
};

class Ball {
public:
	Ball()
		: _body(std::make_shared<Circle>(0.0f, 0.8f, 0.05f))
		, _vx(0.02f)
		, _vy(-0.02f)
	{}
	void update(std::vector<std::shared_ptr<Square>>& squares, std::vector<std::shared_ptr<Square>>& blocks) {
		*_body = _body->move(_vx, _vy);

		for (const auto& square : squares) {
			auto squareCollider = square->collider();
			if (!squareCollider.has_value()) {
				continue;
			}
			int result = CollisionDetectioner::CollidingDirection(squareCollider.value(), _body->collider());
			if (!result && square->isInCollision()) {
				*square = square->outFromCollision();
				continue;
			}
			if (!result) {
				continue;
			}
			if (result && square->isInCollision()) {
				continue;
			}
			if (result & Direction::left) {
				_vx = -_vx;
			}
			else if (result & Direction::right) {
				_vx = -_vx;
			}
			if (result & Direction::top) {
				_vy = -_vy;
			}
			else if (result & Direction::bottom) {
				_vy = -_vy;
			}
			*square = square->intoCollision();
		}

		for (const auto& block : blocks) {
			auto squareCollider = block->collider();
			if (!squareCollider.has_value()) {
				continue;
			}
			int result = CollisionDetectioner::CollidingDirection(squareCollider.value(), _body->collider());
			if (!result && block->isInCollision()) {
				*block = block->outFromCollision();
				continue;
			}
			if (!result) {
				continue;
			}
			if (result && block->isInCollision()) {
				continue;
			}
			if (result & Direction::left) {
				_vx = -_vx;
			}
			else if (result & Direction::right) {
				_vx = -_vx;
			}
			if (result & Direction::top) {
				_vy = -_vy;
			}
			else if (result & Direction::bottom) {
				_vy = -_vy;
			}
			*block = block->destroy();
		}
	}
	static Ball createBall(std::vector<std::shared_ptr<IShape>>& renderShapes) {
		auto ball = Ball();
		renderShapes.push_back(ball._body);
		return ball;
	}
private:
	std::shared_ptr<Circle> _body;
	float _vx;
	float _vy;
};

class Wall {
public:
	Wall(float x1, float y1, float x2, float y2)
		: _body(std::make_shared<Square>(x1, y1, x2, y2))
	{}
	void update() {

	}
	static Wall createWall(float x1, float y1, float x2, float y2, std::vector<std::shared_ptr<IShape>>& renderShapes, std::vector<std::shared_ptr<Square>>& squares) {
		auto wall = Wall(x1, y1, x2, y2);
		renderShapes.push_back(wall._body);
		squares.push_back(wall._body);
		return wall;
	}
private:
	std::shared_ptr<Square> _body;
};

graphic::vulkan::UniformBufferObject nextunifor(int windowWidth, int windowHeight) {
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	graphic::vulkan::UniformBufferObject ubo = {};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f * speed), glm::vec3(0.0f, 1.0f, 0.0f));
	ubo.model *= glm::rotate(glm::mat4(1.0f), time * glm::radians(30.0f * speed), glm::vec3(1.0f, 0.0f, 0.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), windowWidth / (float)windowHeight, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;

	return ubo;
}

int main(int argc, char* argv[]) {
	window::glfw::Window window;
	graphic::vulkan::GraphicEngine engine(window.getWindowHandle());
	input::Mouse mouse(window.getWindowHandle());

	FpsKeeper fpsKeeper(60);

	std::vector<std::shared_ptr<IShape>> shapes;
	std::vector<std::shared_ptr<Square>> squares;
	std::vector<std::shared_ptr<Square>> blocks;

	Player player = Player::createPlayer(shapes, squares);
	std::vector<Block> blockInstances;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			blockInstances.push_back(Block::createBlock(-0.6f + 0.3f * i, -0.8f + 0.2f * j, shapes, blocks));
		}
	}
	Ball ball = Ball::createBall(shapes);
	Wall wallLeft = Wall::createWall(-1.1f, -1.1f, -1.0f, 1.0f, shapes, squares);
	Wall wallTop = Wall::createWall(-1.1f, -1.1f, 1.1f, -1.0f, shapes, squares);
	Wall wallRight = Wall::createWall(1.0f, -1.1f, 1.1f, 1.0f, shapes, squares);

	std::vector<graphic::vulkan::Vertex> allVertices;
	std::vector<uint16_t> allIndices;

	for (auto shape : shapes) {
		auto vv = shape->indices(allVertices.size());
		std::copy(vv.begin(), vv.end(), std::back_inserter(allIndices));
		auto iv = shape->vertices();
		std::copy(iv.begin(), iv.end(), std::back_inserter(allVertices));
	}

	engine.createBuffers(allVertices, allIndices);
	
	while (window.continueMainLoop()) {
		fpsKeeper.frameWait();
		auto uniform = nextunifor(window.getWindowWidth(), window.getWindowHeight());

		player.update(window, mouse);
		ball.update(squares, blocks);

		allVertices.clear();

		for (auto shape : shapes) {
			auto iv = shape->vertices();
			std::copy(iv.begin(), iv.end(), std::back_inserter(allVertices));
		}

		engine.updateVertices(allVertices);
		engine.drawFrame(uniform);
	}
	engine.terminate();
}