/*
 * Example use of boost::astar_search_no_init on an infinite, implicitly-defined graph.
 *
 * The graph type used here is XYGraph, representing an infinite grid of squares.  Each
 * square is connected to its eight neighbors; however, the example shows how to use
 * boost::filtered_graph to make the search take place only along orthogonal edges.
 *
 * based on https://groups.google.com/forum/?fromgroups=#!topic/boost-list/Jz5OOygzW3E
 */

#include "Pathfinder.h"

#include <list>
#include <utility>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include <boost/ref.hpp>
#include <boost/operators.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/astar_search.hpp>

#include "../util/Log.h"

// Boost interface type declarations.
enum class Direction;
struct Vertex;
struct NeighborIterator;
class FilterEdge;
class FilterVertex;
template <typename K, typename V>
class DefaultMap;
struct found_goal;
struct Vertex;
struct VertexGraph;

typedef boost::filtered_graph<VertexGraph, FilterEdge, FilterVertex> Graph;
typedef boost::associative_property_map< DefaultMap<Vertex,unsigned> > DistanceMap;
typedef DefaultMap<Vertex,unsigned> WrappedDistanceMap;

// Boost interface type definitions.
enum class Direction {
    MIN = 0,
    N, S, E, W, NW, NE, SE, SW, NONE
};

/**
 * Used to map vertexes to various constant values.
 */
template <typename K, typename V>
class DefaultMap {
public:
    typedef K key_type;
    typedef V data_type;
    typedef std::pair<K,V> value_type;

    DefaultMap(V const& defaultValue) :
    		defaultValue(defaultValue) {
    }

    V& operator[](K const& k) {
        if (m.find(k) == m.end()) {
            m[k] = defaultValue;
        }
        return m[k];
    }

private:
    std::map<K,V> m;
    V const defaultValue;
};

struct FoundGoal {}; //< exception for termination

struct Vertex : public boost::additive<Vertex,
                                   boost::totally_ordered<Vertex,
                                   boost::equivalent<Vertex> > > {
    Vertex(int x = 0, int y = 0);

    // Same square counts.
    bool adjacentTo(Vertex const& that) const {
        return abs(x - that.x) <= 1 && abs(y - that.y) <= 1;
    }

    Vertex& operator=(Vertex const& that);
    Vertex& operator+=(Vertex const& that);

    bool operator<(Vertex const& that) const {
        return x < that.x || (x == that.x && y < that.y);
    }

    int x;
    int y;

    Vertex neighbor(Direction direction) const;
    std::set<Vertex> allNeighbors() const;
};

/*
 * Model of:
 *  * Graph
 *  * IncidenceGraph
 */
struct VertexGraph {
    VertexGraph() = default;

    // Graph concept requirements
    typedef Vertex								vertex_descriptor;
    typedef std::pair<Vertex, Vertex>			edge_descriptor;
    typedef boost::undirected_tag				directed_category;
    typedef boost::disallow_parallel_edge_tag	edge_parallel_category;
    typedef boost::incidence_graph_tag			traversal_category;

    // IncidenceGraph concept requirements
    typedef NeighborIterator					out_edge_iterator;
    typedef int									degree_size_type;
};

namespace boost {
    template <> struct graph_traits<VertexGraph> {
        typedef VertexGraph G;

        typedef G::vertex_descriptor      vertex_descriptor;
        typedef G::edge_descriptor        edge_descriptor;
        typedef G::out_edge_iterator      out_edge_iterator;

        typedef G::directed_category      directed_category;
        typedef G::edge_parallel_category edge_parallel_category;
        typedef G::traversal_category     traversal_category;

        typedef G::degree_size_type       degree_size_type;

        typedef void in_edge_iterator;
        typedef void vertex_iterator;
        typedef void vertices_size_type;
        typedef void edge_iterator;
        typedef void edges_size_type;
    };
}

/**
 * Placeholder, always returns true.
 */
class FilterEdge {
public:
	FilterEdge() = default;
	FilterEdge(b2World& world, const Vector2f& size);
    bool operator()(std::pair<Vertex,Vertex> const& edge) const {
    	return true;
    }
};

/**
 * Filters vertexes, can be used to block certain fields.
 *
 * This might be quite expensive as Box2D is queried for blocking objects in every step.
 */
class FilterVertex {
public:
	FilterVertex() = default;
	FilterVertex(b2World& world, const Vector2f& size);
    bool operator()(Vertex const& vertex) const;

private:
    /**
     * Callback object for b2World::QueryAABB(). Callback::empty is set to false if the vertex is
     * blocked by a solid, non-moving body.
     */
    class Callback : public b2QueryCallback {
    public:
    	Callback() :
    			empty(true) {
    	};
    	bool ReportFixture(b2Fixture* fixture);
        bool empty; //< Keeps the result of a query, do not call before querying.
    };
private:
    b2World* mWorld;
    Vector2f mSize;
};

/**
 * Allows access to adjacent vertexes.
 */
struct NeighborIterator : public boost::iterator_facade<NeighborIterator,
                                                        std::pair<Vertex,Vertex>,
                                                        boost::forward_traversal_tag,
                                                        std::pair<Vertex,Vertex> > {
public:
    NeighborIterator() = default;
    NeighborIterator(Vertex xy, Direction direction);

    NeighborIterator& operator=(NeighborIterator const& that);

    std::pair<Vertex,Vertex> operator*() const;
    void operator++() {
        direction = static_cast<Direction>(int(direction) + 1);
    }
    bool operator==(NeighborIterator const& that) const {
        return xy == that.xy && direction == that.direction;
    }
    bool equal(NeighborIterator const& that) const {
    	return operator==(that);
    }
    void increment() {
    	operator++();
    }

private:
    Vertex xy;
    Direction direction;
};

struct PredecessorMap {
    PredecessorMap() = default;
    PredecessorMap(PredecessorMap const& that) :
    		m(that.m) {
    }

    typedef Vertex key_type;
    typedef Vertex value_type;
    typedef Vertex & reference_type;
    typedef boost::read_write_property_map_tag category;

    Vertex & operator[](Vertex xy) {
    	return m[xy];
    }

    std::map<Vertex,Vertex> m;
};

/**
 * Throws exception when goal is reached.
 */
class GoalVisitor : public boost::default_astar_visitor {
public:
	GoalVisitor(Vertex goal) :
			m_goal(goal) {
	}
    void examine_vertex(Vertex xy, Graph const& g);
private:
    Vertex m_goal;
};

// Euclidean distance heuristic (square root omitted)
class DistanceHeuristic : public boost::astar_heuristic<Graph, int> {
public:
	DistanceHeuristic(Vertex goal) :
			m_goal(goal) {
	}
    unsigned int operator()(Vertex xy);
private:
    Vertex m_goal;
};

// Boost interface function declarations.
Vertex get(PredecessorMap const& pm, Vertex xy);
void put(PredecessorMap & pm, Vertex key, Vertex value);
// IncidenceGraph concept requirements
std::pair<VertexGraph::out_edge_iterator,
VertexGraph::out_edge_iterator>	out_edges(VertexGraph::vertex_descriptor v, VertexGraph const& g);
VertexGraph::degree_size_type 	out_degree(VertexGraph::vertex_descriptor v, VertexGraph const& g);
VertexGraph::vertex_descriptor 	source(VertexGraph::edge_descriptor e, VertexGraph const& g);
VertexGraph::vertex_descriptor 	target(VertexGraph::edge_descriptor e, VertexGraph const& g);
Vertex vertex(const Vector2f& vector);
Vector2f vector(const Vertex& vertex);

// Constant variables.

static const Vector2f VERTEX_SIZE = Vector2f(10, 10);

// Helper functions.
Vertex vertex(const Vector2f& vector) {
	return Vertex(vector.x / VERTEX_SIZE.x, vector.y / VERTEX_SIZE.y);
}

Vector2f vector(const Vertex& vertex) {
	return Vector2f(vertex.x * VERTEX_SIZE.x, vertex.y * VERTEX_SIZE.y);
}

/**
 * @param world The Box2D world instance to use for collisions.
 */
Pathfinder::Pathfinder(b2World& world) :
		mWorld(world) {
}

/**
 * Returns a path from the position of a Physical to an absolute position. Returns an empty
 * std::vector on failure.
 *
 * The path returned is in reverse order so that the immediate next point to move to can be
 * accessed via pop_back().
 * Returns an empty vector on failure.
 *
 * @param physical The object to be moved.
 * @param to The position to find a path to.
 * @return The path from physical.getPostion() to destination. Empty on failure.
 *
 * TODO: Objects are only recognized partially, examples:
 *    Mostly ignores Cover.
 *    Sometimes moves into wall and instantly out again.
 */
std::vector<Vector2f>
Pathfinder::getPath(Body& physical, const Vector2f& destination) {
    Vertex start(vertex(physical.getPosition()));
    Vertex goal(vertex(destination));

    VertexGraph baseGraph;
    Graph graph(baseGraph, FilterEdge(), FilterVertex(mWorld, physical.getSize()));
    PredecessorMap p;
    WrappedDistanceMap wrappedMap = WrappedDistanceMap(std::numeric_limits<unsigned>::max());
    DistanceMap d = DistanceMap(wrappedMap);
    // Constructor of boost::associative_property_map fails if these are declared inline.
    DefaultMap<std::pair<Vertex,Vertex>,unsigned> weightMap =
    		DefaultMap<std::pair<Vertex,Vertex>,unsigned>(1);
    std::map<Vertex,unsigned> vertexIndexMap;
    std::map<Vertex,unsigned> rankMap;
    std::map<Vertex,boost::default_color_type> colorMap;

    // There does not seem destination be a way destination signal the goal as reached other than throwing an exception.
    try {
        astar_search_no_init(graph,
            start,
            DistanceHeuristic(goal)
            , visitor(GoalVisitor(goal))
            . distance_map(d)
            . predecessor_map(boost::ref(p))
            . weight_map(boost::associative_property_map<DefaultMap<std::pair<Vertex,Vertex>,unsigned> >(
                weightMap))
            . vertex_index_map(boost::associative_property_map< std::map<Vertex,unsigned> >(vertexIndexMap))
            . rank_map(boost::associative_property_map< std::map<Vertex,unsigned> >(rankMap))
            . color_map(boost::associative_property_map< std::map<Vertex,boost::default_color_type> >(
                colorMap))
            . distance_compare(std::less<unsigned>())
            . distance_combine(std::plus<unsigned>())
            );
    } catch(FoundGoal&) { // found a path destination the goal
    	std::vector<Vector2f> shortestPath;
    	shortestPath.push_back(destination);
        for(Vertex xy = goal;; xy = p[xy]) {
            shortestPath.push_back(vector(xy));
            if(p[xy] == xy) {
                break;
            }
        }
        // Remove current position.
        shortestPath.pop_back();
        // Remove next node to avoid possibly moving in opposite direction.
        // This should not cause problems if vertexes are small enough.
        shortestPath.pop_back();
        // Leave it in reverse order so we can just pop the current destination.
        //std::reverse(shortestPath.begin(), shortestPath.end());
        return shortestPath;
    }
    LOG_W("No path from " << vector(start) << " destination " << vector(goal));
    return std::vector<Vector2f>();
}

/**
 * @param world Box2D world used for collisions.
 * @param size Size of the moving object.
 */
FilterVertex::FilterVertex(b2World& world, const Vector2f& size) :
		mWorld(&world),
		mSize(size) {
}

/**
 * Decides which vertexes may be accessed.
 *
 * @param vertex A vertex to be tested.
 * @return True if the vertex may be moved to.
 */
bool
FilterVertex::operator()(Vertex const& vertex) const {
	Callback callback;
	b2AABB aabb;
	// Bottom left.
	aabb.lowerBound = vector(vector(vertex) - 0.5f * mSize);
	// Top right.
	aabb.upperBound = vector(vector(vertex) + 0.5f * mSize);
	mWorld->QueryAABB(&callback, aabb);
    return callback.empty;
}

bool
FilterVertex::Callback::ReportFixture(b2Fixture* fixture) {
	Body& physical = *static_cast<Body*>(fixture->GetBody()->GetUserData());
	if (!physical.isMovable() && physical.isSolid()) {
		empty = false;
		return true;
	}
	return false;
}

unsigned int
DistanceHeuristic::operator()(Vertex xy) {
    int dx = m_goal.x - xy.x;
    int dy = m_goal.y - xy.y;
    unsigned int retval = (unsigned int) dx * dx + dy * dy;
    return retval;
}

Vertex
get(PredecessorMap const& pm, Vertex xy) {
    std::map<Vertex,Vertex>::const_iterator found = pm.m.find(xy);
    return (found != pm.m.end()) ? found->second : xy;
}

void
put(PredecessorMap & pm, Vertex key, Vertex value) {
    pm.m[key] = value;
}

void
GoalVisitor::examine_vertex(Vertex xy, Graph const& g) {
    if(xy == m_goal)
        throw FoundGoal();
}

std::pair<VertexGraph::out_edge_iterator, VertexGraph::out_edge_iterator>
out_edges(VertexGraph::vertex_descriptor v,
          VertexGraph const& g) {
    return std::make_pair(VertexGraph::out_edge_iterator(v, Direction::MIN),
                          VertexGraph::out_edge_iterator(v, Direction::NONE) );
}

VertexGraph::degree_size_type
out_degree(VertexGraph::vertex_descriptor v, VertexGraph const& g) {
    return v.allNeighbors().size();
}

VertexGraph::vertex_descriptor
source(VertexGraph::edge_descriptor e, VertexGraph const& g) {
    return e.first;
}

VertexGraph::vertex_descriptor
target(VertexGraph::edge_descriptor e, VertexGraph const& g) {
    return e.second;
}

NeighborIterator::NeighborIterator(Vertex xy, Direction direction) :
		xy(xy),
		direction(direction) {
}

NeighborIterator&
NeighborIterator::operator=(NeighborIterator const& that) {
    xy = that.xy;
    direction = that.direction;
    return *this;
}

std::pair<Vertex,Vertex>
NeighborIterator::operator*() const {
    std::pair<Vertex,Vertex> const retval = std::make_pair(xy, xy.neighbor(direction));
    return retval;
}

Vertex::Vertex(int x, int y) :
		x(x),
		y(y) {
}

Vertex&
Vertex::operator=(Vertex const& that) {
    x = that.x;
    y = that.y;
    return *this;
}

Vertex&
Vertex::operator+=(Vertex const& that) {
    x += that.x;
    y += that.y;
    return *this;
}

Vertex
Vertex::neighbor(Direction direction) const {
    int dx = 0, dy = 0;
    switch (direction)
    {
    case Direction::NW:
    case Direction::W:
    case Direction::SW:
        dx = -1;
        break;
    case Direction::NE:
    case Direction::E:
    case Direction::SE:
        dx = 1;
        break;
    default:
        break;
    }
    switch (direction)
    {
    case Direction::NW:
    case Direction::N:
    case Direction::NE:
        dy = -1;
        break;
    case Direction::SW:
    case Direction::S:
    case Direction::SE:
        dy = 1;
        break;
    default:
        break;
    }
    Vertex const neighbor(x + dx, y + dy);
    return neighbor;
}

std::set<Vertex>
Vertex::allNeighbors() const {
    std::set<Vertex> neighbors;

    for (int dx = -1; dx <= 1; ++dx)
        for (int dy = -1; dy <= 1; ++dy)
            neighbors.insert(Vertex(x+dx,y+dy));

    return neighbors;
}
