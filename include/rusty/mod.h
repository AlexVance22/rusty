#pragma once
#include "base.h"
#include "ops.h"
#include "mem.h"
#include "box.h"
#include "option.h"
#include "result.h"
#include "str.h"
#include "stralloc.h"
#include "vec.h"

// using rstd::marker::{Copy, Send, Sized, Sync, Unpin};
// using rstd::ops::{Drop, Fn, FnMut, FnOnce};
using rstd::mem::drop;
using rstd::boxed::Box;
// using rstd::borrow::ToOwned;
// using rstd::clone::Clone;
using rstd::cmp::PartialEq; // PartialOrd, Eq, Ord};
// using rstd::convert::{AsRef, AsMut, Into, From};
// using rstd::def::Default;
// using rstd::iter::{Iterator, Extend, IntoIterator, DoubleEndedIterator, ExactSizeIterator};
using rstd::option::Option; // {self, Some, None};
using rstd::result::Result; // {self, Ok, Err};
using rstd::string::String; using rstd::string::ToString;
using rstd::vec::Vec;
// using rstd::convert::{TryFrom, TryInto};
// using rstd::iter::FromIterator;

