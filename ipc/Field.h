#pragma once
#include <cstdint>
#include <string>

#define FIELD( name, type ) \
   private: \
      type name; \
   public: \
      const type& Get##name() const { return name; }; \
      void Set##name( const type& value ) { name = value; };
