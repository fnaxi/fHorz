// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

#define FORCEINLINE __forceinline

/**
 * Used to mimic C#'s "=>" syntax.
 *
 * @example
 * // C#
 * public bool IsValid() => bValid;
 * // C++
 * bool IsValid() const arrow (bValid)
 */
#define arrow(x) { return x; }
