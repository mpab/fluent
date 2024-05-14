// copyright (c) 2017- mpab

#pragma once

#include <algorithm>
#include <cmath>

#include "boost/variant.hpp"
#include "logger.hpp"
#include "node.hpp"

namespace op {
using namespace boost;
using namespace node;

///////////////////////////////////////////////////////////////////////////

template <typename T, typename O>
struct op_vis_1 : public boost::static_visitor<T> {
  T operator()(int64_t& a) const { return O().operator()(a); }

  T operator()(double& a) const { return O().operator()(a); }

  T operator()(string& a) const { return O().operator()(a); }
};

template <typename O, typename T>
T* op(const char* fn, T* a) {
  if (!a) {
    logger::error() << fn << " bad val" << endl;
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

  T operator()(int64_t& a, string& b) const { return O().operator()(a, b); }

  T operator()(double& a, double& b) const { return O().operator()(a, b); }

  T operator()(double& a, int64_t& b) const { return O().operator()(a, b); }

  T operator()(double& a, string& b) const { return O().operator()(a, b); }

  T operator()(string& a, string& b) const { return O().operator()(a, b); }

  T operator()(string& a, int64_t& b) const { return O().operator()(a, b); }

  T operator()(string& a, double& b) const { return O().operator()(a, b); }
};

template <typename O, typename T>
T* op(const char* fn, T* a, T* b) {
  if (!a || !b) {
    if (!a) logger::warn() << fn << " bad lval" << endl;
    if (!b) logger::warn() << fn << " bad rval" << endl;
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

  T operator()(string& a) const {
    auto copy(a);
    reverse(copy.begin(), copy.end());
    return T(copy);
  }
};
using negate_v = op_vis_1<Value, negate_op<Value> >;

Variable* (&xnegate)(const char*, Variable*) = op<negate_v>;
#define negate(a) xnegate("negate:", a)

///////////////////////////////////////////////////////////////////////////

struct conditional_op {
  bool operator()(int64_t& a) const { return a != 0; }

  bool operator()(double& a) const { return a != 0; }

  bool operator()(string& a) const { return !a.empty(); }
};

using conditional_v = op_vis_1<bool, conditional_op>;

bool conditional(Variable* a) {
  return apply_visitor(conditional_v(), a->value);
}
///////////////////////////////////////////////////////////////////////////

template <typename T>
struct add_op {
  T operator()(int64_t& a, int64_t& b) const { return T(a + b); }

  T operator()(int64_t& a, double& b) const { return T(a + b); }

  T operator()(int64_t& a, string& b) const { return T(); }

  T operator()(double& a, double& b) const { return T(a + b); }

  T operator()(double& a, int64_t& b) const { return T(a + b); }

  T operator()(double& a, string& b) const { return T(); }

  T operator()(string& a, string& b) const { return T(a + b); }

  T operator()(string& a, int64_t& b) const { return T(); }

  T operator()(string& a, double& b) const { return T(); }
};
using add_v = op_vis_2<Value, add_op<Value> >;

Variable* (&xadd)(const char*, Variable*, Variable*) = op<add_v>;
#define add(a, b) xadd("add:", a, b)

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct sub_op {
  T operator()(int64_t& a, int64_t& b) const { return T(a - b); }

  T operator()(int64_t& a, double& b) const { return T(a - b); }

  T operator()(int64_t& a, string& b) const { return T(); }

  T operator()(double& a, double& b) const { return T(a - b); }

  T operator()(double& a, int64_t& b) const { return T(a - b); }

  T operator()(double& a, string& b) const { return T(); }

  T operator()(string& a, string& b) const { return T(); }

  T operator()(string& a, int64_t& b) const { return T(); }

  T operator()(string& a, double& b) const { return T(); }
};
using sub_v = op_vis_2<Value, sub_op<Value> >;

Variable* (&xsub)(const char*, Variable*, Variable*) = op<sub_v>;
#define sub(a, b) xsub("sub:", a, b)

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct mul_op {
  T operator()(int64_t& a, int64_t& b) const { return T(a * b); }

  T operator()(int64_t& a, double& b) const { return T(a * b); }

  T operator()(int64_t& a, string& b) const { return T(); }

  T operator()(double& a, double& b) const { return T(a * b); }

  T operator()(double& a, int64_t& b) const { return T(a * b); }

  T operator()(double& a, string& b) const { return T(); }

  T operator()(string& a, string& b) const { return T(); }

  T operator()(string& a, int64_t& b) const { return T(); }

  T operator()(string& a, double& b) const { return T(); }
};
using mul_v = op_vis_2<Value, mul_op<Value> >;

Variable* (&xmul)(const char*, Variable*, Variable*) = op<mul_v>;
#define mul(a, b) xmul("mul:", a, b)

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct div_op {
  T operator()(int64_t& a, int64_t& b) const { return T(a / b); }

  T operator()(int64_t& a, double& b) const { return T(a / b); }

  T operator()(int64_t& a, string& b) const { return T(); }

  T operator()(double& a, double& b) const { return T(a / b); }

  T operator()(double& a, int64_t& b) const { return T(a / b); }

  T operator()(double& a, string& b) const { return T(); }

  T operator()(string& a, string& b) const { return T(); }

  T operator()(string& a, int64_t& b) const { return T(); }

  T operator()(string& a, double& b) const { return T(); }
};
using div_v = op_vis_2<Value, div_op<Value> >;

Variable* (&xdiv)(const char*, Variable*, Variable*) = op<div_v>;
#define div(a, b) xdiv("div:", a, b)

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct gt_op {
  T operator()(int64_t& a, int64_t& b) const { return T((int64_t)(a > b)); }

  T operator()(int64_t& a, double& b) const { return T((int64_t)(a > b)); }

  T operator()(int64_t& a, string& b) const { return T(); }

  T operator()(double& a, double& b) const { return T((double)(a > b)); }

  T operator()(double& a, int64_t& b) const { return T((double)(a > b)); }

  T operator()(double& a, string& b) const { return T(); }

  T operator()(string& a, string& b) const { return T(); }

  T operator()(string& a, int64_t& b) const { return T(); }

  T operator()(string& a, double& b) const { return T(); }
};
using gt_v = op_vis_2<Value, gt_op<Value> >;

Variable* (&xgt)(const char*, Variable*, Variable*) = op<gt_v>;
#define gt(a, b) xgt("gt:", a, b)

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct ge_op {
  T operator()(int64_t& a, int64_t& b) const { return T((int64_t)(a >= b)); }

  T operator()(int64_t& a, double& b) const { return T((int64_t)(a >= b)); }

  T operator()(int64_t& a, string& b) const { return T(); }

  T operator()(double& a, double& b) const { return T((double)(a >= b)); }

  T operator()(double& a, int64_t& b) const { return T((double)(a >= b)); }

  T operator()(double& a, string& b) const { return T(); }

  T operator()(string& a, string& b) const { return T(); }

  T operator()(string& a, int64_t& b) const { return T(); }

  T operator()(string& a, double& b) const { return T(); }
};
using ge_v = op_vis_2<Value, ge_op<Value> >;

Variable* (&xge)(const char*, Variable*, Variable*) = op<ge_v>;
#define ge(a, b) xge("ge:", a, b)

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct lt_op {
  T operator()(int64_t& a, int64_t& b) const { return T((int64_t)(a < b)); }

  T operator()(int64_t& a, double& b) const { return T((int64_t)(a < b)); }

  T operator()(int64_t& a, string& b) const { return T(); }

  T operator()(double& a, double& b) const { return T((double)(a < b)); }

  T operator()(double& a, int64_t& b) const { return T((double)(a < b)); }

  T operator()(double& a, string& b) const { return T(); }

  T operator()(string& a, string& b) const { return T(); }

  T operator()(string& a, int64_t& b) const { return T(); }

  T operator()(string& a, double& b) const { return T(); }
};
using lt_v = op_vis_2<Value, lt_op<Value> >;

Variable* (&xlt)(const char*, Variable*, Variable*) = op<lt_v>;
#define lt(a, b) xlt("lt:", a, b)

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct le_op {
  T operator()(int64_t& a, int64_t& b) const { return T((int64_t)(a <= b)); }

  T operator()(int64_t& a, double& b) const { return T((int64_t)(a <= b)); }

  T operator()(int64_t& a, string& b) const { return T(); }

  T operator()(double& a, double& b) const { return T((double)(a <= b)); }

  T operator()(double& a, int64_t& b) const { return T((double)(a <= b)); }

  T operator()(double& a, string& b) const { return T(); }

  T operator()(string& a, string& b) const { return T(); }

  T operator()(string& a, int64_t& b) const { return T(); }

  T operator()(string& a, double& b) const { return T(); }
};
using le_v = op_vis_2<Value, le_op<Value> >;

Variable* (&xle)(const char*, Variable*, Variable*) = op<le_v>;
#define le(a, b) xle("le:", a, b)

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct eq_op {
  T operator()(int64_t& a, int64_t& b) const { return T((int64_t)(a == b)); }

  T operator()(int64_t& a, double& b) const { return T((int64_t)(a == b)); }

  T operator()(int64_t& a, string& b) const { return T(); }

  T operator()(double& a, double& b) const { return T((double)(a == b)); }

  T operator()(double& a, int64_t& b) const { return T((double)(a == b)); }

  T operator()(double& a, string& b) const { return T(); }

  T operator()(string& a, string& b) const { return T(int64_t(a == b)); }

  T operator()(string& a, int64_t& b) const { return T(); }

  T operator()(string& a, double& b) const { return T(); }
};
using eq_v = op_vis_2<Value, eq_op<Value> >;

Variable* (&xeq)(const char*, Variable*, Variable*) = op<eq_v>;
#define is_eq(a, b) xeq("eq:", a, b)

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct ne_op {
  T operator()(int64_t& a, int64_t& b) const { return T((int64_t)(a != b)); }

  T operator()(int64_t& a, double& b) const { return T((int64_t)(a != b)); }

  T operator()(int64_t& a, string& b) const { return T(); }

  T operator()(double& a, double& b) const { return T((double)(a != b)); }

  T operator()(double& a, int64_t& b) const { return T((double)(a != b)); }

  T operator()(double& a, string& b) const { return T(); }

  T operator()(string& a, string& b) const { return T(int64_t(a != b)); }

  T operator()(string& a, int64_t& b) const { return T(); }

  T operator()(string& a, double& b) const { return T(); }
};
using ne_v = op_vis_2<Value, ne_op<Value> >;

Variable* (&xne)(const char*, Variable*, Variable*) = op<ne_v>;
#define ne(a, b) xne("neq:", a, b)

///////////////////////////////////////////////////////////////////////////

template <typename T>
struct exp_op {
  T operator()(int64_t& a, int64_t& b) const { return T(pow(a, b)); }

  T operator()(int64_t& a, double& b) const { return T(pow(a, b)); }

  T operator()(int64_t& a, string& b) const { return T(); }

  T operator()(double& a, double& b) const { return T(pow(a, b)); }

  T operator()(double& a, int64_t& b) const { return T(pow(a, b)); }

  T operator()(double& a, string& b) const { return T(); }

  T operator()(string& a, string& b) const { return T(); }

  T operator()(string& a, int64_t& b) const { return T(); }

  T operator()(string& a, double& b) const { return T(); }
};
using exp_v = op_vis_2<Value, exp_op<Value> >;

Variable* (&xexp)(const char*, Variable*, Variable*) = op<exp_v>;
#define exp(a, b) xexp("exp:", a, b)

}  // namespace op
