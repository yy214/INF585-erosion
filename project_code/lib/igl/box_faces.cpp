#include "box_faces.h"
#include "AABB.h"
#include <vector>
#include <utility>

template <typename DerivedV, typename DerivedQ>
IGL_INLINE void igl::box_faces(
  const Eigen::AlignedBox<typename DerivedV::Scalar,3> & box,
  const typename DerivedV::Scalar shrink,
  Eigen::PlainObjectBase<DerivedV> & P,
  Eigen::PlainObjectBase<DerivedQ> & Q)
{
  auto min_corner = box.min();
  auto max_corner = box.max();
  // shrink by 3%
  min_corner = min_corner + shrink*(max_corner-min_corner);
  max_corner = max_corner - shrink*(max_corner-min_corner);
  P.resize(8,3);
  Q.resize(6,4);
  int p = 0;
  int q = 0;
  Q.row(q++) << p+0,p+1,p+2,p+3;
  Q.row(q++) << p+0,p+1,p+5,p+4;
  Q.row(q++) << p+1,p+2,p+6,p+5;
  Q.row(q++) << p+2,p+3,p+7,p+6;
  Q.row(q++) << p+3,p+0,p+4,p+7;
  Q.row(q++) << p+4,p+5,p+6,p+7;
  P.row(p++) = min_corner;
  P.row(p++) = Eigen::RowVector3d(max_corner[0],min_corner[1],min_corner[2]);
  P.row(p++) = Eigen::RowVector3d(max_corner[0],max_corner[1],min_corner[2]);
  P.row(p++) = Eigen::RowVector3d(min_corner[0],max_corner[1],min_corner[2]);
  P.row(p++) = Eigen::RowVector3d(min_corner[0],min_corner[1],max_corner[2]);
  P.row(p++) = Eigen::RowVector3d(max_corner[0],min_corner[1],max_corner[2]);
  P.row(p++) = max_corner;
  P.row(p++) = Eigen::RowVector3d(min_corner[0],max_corner[1],max_corner[2]);
}

template <
  typename DerivedV,
  typename DerivedP,
  typename DerivedQ,
  typename DerivedD >
IGL_INLINE void igl::box_faces(
  const igl::AABB<DerivedV,3> & tree,
  Eigen::PlainObjectBase<DerivedP> & P,
  Eigen::PlainObjectBase<DerivedQ> & Q,
  Eigen::PlainObjectBase<DerivedD> & D)
{
  const int num_nodes = tree.size();
  P.resize(8*num_nodes,3);
  Q.resize(6*num_nodes,4);
  D.resize(6*num_nodes);
  int d = 0;
  int p = 0;
  int q = 0;
  std::vector<std::pair<const igl::AABB<DerivedV,3> *,int> > stack;
  stack.push_back({&tree,0});
  while(!stack.empty())
  {
    const auto pair = stack.back();
    const auto * node = pair.first;
    const int depth = pair.second;
    D(d++) = depth;
    D(d++) = depth;
    D(d++) = depth;
    D(d++) = depth;
    D(d++) = depth;
    D(d++) = depth;
    stack.pop_back();
    const auto & box = node->m_box;

    DerivedP Pi;
    DerivedQ Qi;
    box_faces(box,0.03,Pi,Qi);
    P.block(p,0,8,3) = Pi;
    Q.block(q,0,6,4) = Qi.array()+p;
    p += 8;
    q += 6;
    if(node->m_left)
    {
      stack.push_back({node->m_left,depth+1});
    }
    if(node->m_right)
    {
      stack.push_back({node->m_right,depth+1});
    }
  }
}

#ifdef IGL_STATIC_LIBRARY
// Explicit template instantiation
// generated by autoexplicit.sh
template void igl::box_faces<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, 1, 0, -1, 1> >(igl::AABB<Eigen::Matrix<double, -1, -1, 0, -1, -1>, 3> const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> >&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> >&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 1, 0, -1, 1> >&);
#endif
