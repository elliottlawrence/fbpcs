/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <emp-sh2pc/emp-sh2pc.h>
#include <folly/dynamic.h>
#include <vector>
#include "folly/logging/xlog.h"

namespace aggregation::private_aggregation {

struct AttributionResult {
  const bool isAttributed;

  // privatelyShareArrayFrom support
  friend bool operator==(
      const AttributionResult& a,
      const AttributionResult& b) {
    return a.isAttributed == b.isAttributed;
  }
  friend std::ostream& operator<<(
      std::ostream& os,
      const AttributionResult& ar) {
    return os << (ar.isAttributed ? "Attributed{" : "Not Attributed{") << "}";
  }

  static AttributionResult fromDynamic(const folly::dynamic& obj) {
    return AttributionResult{obj["is_attributed"].asBool()};
  }
};

struct PrivateAttributionResult {
  emp::Bit isAttributed;

  explicit PrivateAttributionResult(AttributionResult res, int party)
      : PrivateAttributionResult(emp::Bit{res.isAttributed, party}) {}

  explicit PrivateAttributionResult(const emp::Bit& _isAttributed)
      : isAttributed{_isAttributed} {}

  // string conversion support
  template <typename T = std::string>
  T reveal(int party) const {
    std::stringstream out;
    out << (isAttributed.reveal<bool>(party) ? "Attributed{"
                                             : "Not Attributed{")
        << "}";

    return out.str();
  }
};

} // namespace aggregation::private_aggregation
