// copyright (c) 2017- mpab

#pragma once

#include <algorithm>
#include <cmath>

#include "logger.hpp"
#include "node.hpp"

namespace op {

///////////////////////////////////////////////////////////////////////////

template <typename T, typename O>
struct op_vis_1 : public boost::static_visitor<T> {
  T operator()(int64_t& a) const { return O().operator()(a); }

  T operator()(double& a) const { return O().operator()(a); }

  T operator()(std::string& a) const { return O().operator()(a); }
};

template <typename O, typename T>
T* op(const char* fn, T* a) {
  if (!a) {
    logger::error() << fn << " bad val" << std::endl;
    return nullptr;
  }
  auto r = apply_visitor(O(), a->value);
  return new T(&r);
}

///////////////////////////////////////////////////////////////////////////

template <typename T, typename O>
struct op_vis_2 : public boost::static_visitor<T> {
  T operator()(int64_t& a, int64_t& b) const { return O().operator()(a, b); }

  T operator()(int64_t& a, double& b) const { return O().operator()(a, b); }

  T operator()(int64_t& a, std::string& b) const {
    return O().operator()(a, b);
  }

  T operator()(double& a, double& b) const { return O().operator()(a, b); }

  T operator()(double& a, int64_t& b) const { return O().operator()(a, b); }

  T operator()(double& a, std::string& b) const { return O().operator()(a, b); }

  T operator()(std::string& a, std::string& b) const {
    return O().operator()(a, b);
  }

  T operator()(std::string& a, int64_t& b) const {
    return O().operator()(a, b);
  }

  T operator()(std::string& a, double& b) const { return O().operator()(a, b); }
};

template <typename O, typename T>
T* op(const char* fn, T* a, T* b) {
  if (!a || !b) {
    if (!a) logger::warn() << fn << " bad lval" << std::endl;
    if (!b) logger::warn() << fn << " bad rval" << std::endl;
    return nullptr;
  }
  auto r = apply_visitor(O(), a->value, b->value);
  return new T(&r);
}

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct negate_op {
  T operator()(int64_t& a) const { return T(-a); }

  T operator()(double& a) const { return T(-a); }

  T operator()(std::string& a) const {
    auto copy(a);
    reverse(copy.begin(), copy.end());
    return T(copy);
  }
};
using negate_v = op_vis_1<node::Value, negate_op<node::Value> >;

node::Variable* (&xnegate)(const char*, node::Variable*) = op<negate_v>;
#define negate(a) xnegate("negate:", a)

///////////////////////////////////////////////////////////////////////////

struct conditional_op {
  bool operator()(int64_t& a) const { return a != 0; }

  bool operator()(double& a) const { return a != 0; }

  bool operator()(std::string& a) const { return !a.empty(); }
};

using conditional_v = op_vis_1<bool, conditional_op>;

bool conditional(node::Variable* a) {
  return apply_visitor(conditional_v(), a->value);
}
///////////////////////////////////////////////////////////////////////////

template <typename T>
struct add_op {
  T operator()(int64_t& a, int64_t& b) const { return T(a + b); }

  T operator()(int64_t& a, double& b) const { return T(a + b); }

  T operator()(int64_t& a, std::string& b) const { return T(); }

  T operator()(double& a, double& b) const { return T(a + b); }

  T operator()(double& a, int64_t& b) const { return T(a + b); }

  T operator()(double& a, std::string& b) const { return T(); }

  T operator()(std::string& a, std::string& b) const { return T(a + b); }

  T operator()(std::string& a, int64_t& b) const { return T(); }

  T operator()(std::string& a, double& b) const { return T(); }
};
using add_v = op_vis_2<node::Value, add_op<node::Value> >;

node::Variable* (&xadd)(const char*, node::Variable*,
                        node::Variable*) = op<add_v>;
#define add(a, b) xadd("add:", a, b)

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct sub_op {
  T operator()(int64_t& a, int64_t& b) const { return T(a - b); }

  T operator()(int64_t& a, double& b) const { return T(a - b); }

  T operator()(int64_t& a, std::string& b) const { return T(); }

  T operator()(double& a, double& b) const { return T(a - b); }

  T operator()(double& a, int64_t& b) const { return T(a - b); }

  T operator()(double& a, std::string& b) const { return T(); }

  T operator()(std::string& a, std::string& b) const { return T(); }

  T operator()(std::string& a, int64_t& b) const { return T(); }

  T operator()(std::string& a, double& b) const { return T(); }
};
using sub_v = op_vis_2<node::Value, sub_op<node::Value> >;

node::Variable* (&xsub)(const char*, node::Variable*,
                        node::Variable*) = op<sub_v>;
#define sub(a, b) xsub("sub:", a, b)

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct mul_op {
  T operator()(int64_t& a, int64_t& b) const { return T(a * b); }

  T operator()(int64_t& a, double& b) const { return T(a * b); }

  T operator()(int64_t& a, std::string& b) const { return T(); }

  T operator()(double& a, double& b) const { return T(a * b); }

  T operator()(double& a, int64_t& b) const { return T(a * b); }

  T operator()(double& a, std::string& b) const { return T(); }

  T operator()(std::string& a, std::string& b) const { return T(); }

  T operator()(std::string& a, int64_t& b) const { return T(); }

  T operator()(std::string& a, double& b) const { return T(); }
};
using mul_v = op_vis_2<node::Value, mul_op<node::Value> >;

node::Variable* (&xmul)(const char*, node::Variable*,
                        node::Variable*) = op<mul_v>;
#define mul(a, b) xmul("mul:", a, b)

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct div_op {
  T operator()(int64_t& a, int64_t& b) const { return T(a / b); }

  T operator()(int64_t& a, double& b) const { return T(a / b); }

  T operator()(int64_t& a, std::string& b) const { return T(); }

  T operator()(double& a, double& b) const { return T(a / b); }

  T operator()(double& a, int64_t& b) const { return T(a / b); }

  T operator()(double& a, std::string& b) const { return T(); }

  T operator()(std::string& a, std::string& b) const { return T(); }

  T operator()(std::string& a, int64_t& b) const { return T(); }

  T operator()(std::string& a, double& b) const { return T(); }
};
using div_v = op_vis_2<node::Value, div_op<node::Value> >;

node::Variable* (&xdiv)(const char*, node::Variable*,
                        node::Variable*) = op<div_v>;
#define div(a, b) xdiv("div:", a, b)

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct gt_op {
  T operator()(int64_t& a, int64_t& b) const { return T((int64_t)(a > b)); }

  T operator()(int64_t& a, double& b) const { return T((int64_t)(a > b)); }

  T operator()(int64_t& a, std::string& b) const { return T(); }

  T operator()(double& a, double& b) const { return T((double)(a > b)); }

  T operator()(double& a, int64_t& b) const { return T((double)(a > b)); }

  T operator()(double& a, std::string& b) const { return T(); }

  T operator()(std::string& a, std::string& b) const { return T(); }

  T operator()(std::string& a, int64_t& b) const { return T(); }

  T operator()(std::string& a, double& b) const { return T(); }
};
using gt_v = op_vis_2<node::Value, gt_op<node::Value> >;

node::Variable* (&xgt)(const char*, node::Variable*,
                       node::Variable*) = op<gt_v>;
#define gt(a, b) xgt("gt:", a, b)

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct ge_op {
  T operator()(int64_t& a, int64_t& b) const { return T((int64_t)(a >= b)); }

  T operator()(int64_t& a, double& b) const { return T((int64_t)(a >= b)); }

  T operator()(int64_t& a, std::string& b) const { return T(); }

  T operator()(double& a, double& b) const { return T((double)(a >= b)); }

  T operator()(double& a, int64_t& b) const { return T((double)(a >= b)); }

  T operator()(double& a, std::string& b) const { return T(); }

  T operator()(std::string& a, std::string& b) const { return T(); }

  T operator()(std::string& a, int64_t& b) const { return T(); }

  T operator()(std::string& a, double& b) const { return T(); }
};
using ge_v = op_vis_2<node::Value, ge_op<node::Value> >;

node::Variable* (&xge)(const char*, node::Variable*,
                       node::Variable*) = op<ge_v>;
#define ge(a, b) xge("ge:", a, b)

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct lt_op {
  T operator()(int64_t& a, int64_t& b) const { return T((int64_t)(a < b)); }

  T operator()(int64_t& a, double& b) const { return T((int64_t)(a < b)); }

  T operator()(int64_t& a, std::string& b) const { return T(); }

  T operator()(double& a, double& b) const { return T((double)(a < b)); }

  T operator()(double& a, int64_t& b) const { return T((double)(a < b)); }

  T operator()(double& a, std::string& b) const { return T(); }

  T operator()(std::string& a, std::string& b) const { return T(); }

  T operator()(std::string& a, int64_t& b) const { return T(); }

  T operator()(std::string& a, double& b) const { return T(); }
};
using lt_v = op_vis_2<node::Value, lt_op<node::Value> >;

node::Variable* (&xlt)(const char*, node::Variable*,
                       node::Variable*) = op<lt_v>;
#define lt(a, b) xlt("lt:", a, b)

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct le_op {
  T operator()(int64_t& a, int64_t& b) const { return T((int64_t)(a <= b)); }

  T operator()(int64_t& a, double& b) const { return T((int64_t)(a <= b)); }

  T operator()(int64_t& a, std::string& b) const { return T(); }

  T operator()(double& a, double& b) const { return T((double)(a <= b)); }

  T operator()(double& a, int64_t& b) const { return T((double)(a <= b)); }

  T operator()(double& a, std::string& b) const { return T(); }

  T operator()(std::string& a, std::string& b) const { return T(); }

  T operator()(std::string& a, int64_t& b) const { return T(); }

  T operator()(std::string& a, double& b) const { return T(); }
};
using le_v = op_vis_2<node::Value, le_op<node::Value> >;

node::Variable* (&xle)(const char*, node::Variable*,
                       node::Variable*) = op<le_v>;
#define le(a, b) xle("le:", a, b)

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct eq_op {
  T operator()(int64_t& a, int64_t& b) const { return T((int64_t)(a == b)); }

  T operator()(int64_t& a, double& b) const { return T((int64_t)(a == b)); }

  T operator()(int64_t& a, std::string& b) const { return T(); }

  T operator()(double& a, double& b) const { return T((double)(a == b)); }

  T operator()(double& a, int64_t& b) const { return T((double)(a == b)); }

  T operator()(double& a, std::string& b) const { return T(); }

  T operator()(std::string& a, std::string& b) const {
    return T(int64_t(a == b));
  }

  T operator()(std::string& a, int64_t& b) const { return T(); }

  T operator()(std::string& a, double& b) const { return T(); }
};
using eq_v = op_vis_2<node::Value, eq_op<node::Value> >;

node::Variable* (&xeq)(const char*, node::Variable*,
                       node::Variable*) = op<eq_v>;
#define is_eq(a, b) xeq("eq:", a, b)

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct ne_op {
  T operator()(int64_t& a, int64_t& b) const { return T((int64_t)(a != b)); }

  T operator()(int64_t& a, double& b) const { return T((int64_t)(a != b)); }

  T operator()(int64_t& a, std::string& b) const { return T(); }

  T operator()(double& a, double& b) const { return T((double)(a != b)); }

  T operator()(double& a, int64_t& b) const { return T((double)(a != b)); }

  T operator()(double& a, std::string& b) const { return T(); }

  T operator()(std::string& a, std::string& b) const {
    return T(int64_t(a != b));
  }

  T operator()(std::string& a, int64_t& b) const { return T(); }

  T operator()(std::string& a, double& b) const { return T(); }
};
using ne_v = op_vis_2<node::Value, ne_op<node::Value> >;

node::Variable* (&xne)(const char*, node::Variable*,
                       node::Variable*) = op<ne_v>;
#define ne(a, b) xne("neq:", a, b)

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct exp_op {
  T operator()(int64_t& a, int64_t& b) const { return T(pow(a, b)); }

  T operator()(int64_t& a, double& b) const { return T(pow(a, b)); }

  T operator()(int64_t& a, std::string& b) const { return T(); }

  T operator()(double& a, double& b) const { return T(pow(a, b)); }

  T operator()(double& a, int64_t& b) const { return T(pow(a, b)); }

  T operator()(double& a, std::string& b) const { return T(); }

  T operator()(std::string& a, std::string& b) const { return T(); }

  T operator()(std::string& a, int64_t& b) const { return T(); }

  T operator()(std::string& a, double& b) const { return T(); }
};
using exp_v = op_vis_2<node::Value, exp_op<node::Value> >;

node::Variable* (&xexp)(const char*, node::Variable*,
                        node::Variable*) = op<exp_v>;
#define exp(a, b) xexp("exp:", a, b)

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct mod_op {
  T operator()(int64_t& a, int64_t& b) const { return T(a % b); }

  T operator()(int64_t& a, double& b) const { return T(); }

  T operator()(int64_t& a, std::string& b) const { return T(); }

  T operator()(double& a, double& b) const { return T(); }

  T operator()(double& a, int64_t& b) const { return T(); }

  T operator()(double& a, std::string& b) const { return T(); }

  T operator()(std::string& a, std::string& b) const { return T(); }

  T operator()(std::string& a, int64_t& b) const { return T(); }

  T operator()(std::string& a, double& b) const { return T(); }
};
using mod_v = op_vis_2<node::Value, mod_op<node::Value> >;

node::Variable* (&xmod)(const char*, node::Variable*,
                        node::Variable*) = op<mod_v>;
#define mod(a, b) xmod("exp:", a, b)

}  // namespace op
