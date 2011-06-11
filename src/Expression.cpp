////////////////////////////////////////////////////////////////////////////////
// taskwarrior - a command line task list manager.
//
// Copyright 2006 - 2011, Paul Beckingham, Federico Hernandez.
// All rights reserved.
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the
//
//     Free Software Foundation, Inc.,
//     51 Franklin Street, Fifth Floor,
//     Boston, MA
//     02110-1301
//     USA
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream> // TODO Remove.
#include <sstream>
#include <Context.h>
#include <Lexer.h>
#include <Date.h>
#include <Duration.h>
#include <Variant.h>
#include <text.h>
#include <Expression.h>

extern Context context;

////////////////////////////////////////////////////////////////////////////////
// Perform all the necessary steps prior to an eval call.
Expression::Expression (Arguments& arguments)
: _args (arguments)
{
  _args.dump ("Expression::Expression");

  expand_sequence ();
  implicit_and ();
  expand_tag ();
  expand_pattern ();      // Configurable
  expand_attr ();
  expand_attmod ();
  expand_word ();
  expand_expression ();   // Configurable
  postfix ();
}

////////////////////////////////////////////////////////////////////////////////
Expression::~Expression ()
{
}

////////////////////////////////////////////////////////////////////////////////
bool Expression::eval (Task& task)
{
  std::vector <Variant> value_stack;

  std::vector <std::pair <std::string, std::string> >::iterator arg;
  for (arg = _args.begin (); arg != _args.end (); ++arg)
  {
    // if (arg->second != "op")
    //   value_stack.push_back (Variant (*arg));

    // else
    // {
    //   if (arg->first == "+")
    //   {
    //     pop
    //     pop
    //     add the two operands
    //     push result
    //   }
    //   else if (arg->first == "?")
    //   {
    //   }
    //   else if (arg->first == "?")
    //   {
    //   }
    //   else
    //    throw std::string ("Unsupported operator '") + arg->first + "'.";
    // }
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////
// Convert:  1,3-5,00000000-0000-0000-0000-000000000000
//
// To:       (id=1 or (id>=3 and id<=5) or
//            uuid="00000000-0000-0000-0000-000000000000")
void Expression::expand_sequence ()
{
  Arguments temp;

  // Extract all the components of a sequence.
  std::vector <int> ids;
  std::vector <std::string> uuids;
  std::vector <std::pair <std::string, std::string> >::iterator arg;
  for (arg = _args.begin (); arg != _args.end (); ++arg)
  {
    if (arg->second == "id")
      Arguments::extract_id (arg->first, ids);

    else if (arg->second == "uuid")
      Arguments::extract_uuid (arg->first, uuids);
  }

  // If there is no sequence, we're done.
  if (ids.size () == 0 && uuids.size () == 0)
    return;

  // Construct the algebraic form.
  std::stringstream sequence;
  sequence << "(";
  for (unsigned int i = 0; i < ids.size (); ++i)
  {
    if (i)
      sequence << " or ";

    sequence << "id=" << ids[i];
  }

  if (uuids.size ())
  {
    if (sequence.str ().length () > 1)
      sequence << " or ";

    for (unsigned int i = 0; i < uuids.size (); ++i)
    {
      if (i)
        sequence << " or ";

      sequence << "uuid=\"" << uuids[i] << "\"";
    }
  }

  sequence << ")";

  // Copy everything up to the first id/uuid.
  for (arg = _args.begin (); arg != _args.end (); ++arg)
  {
    if (arg->second == "id" || arg->second == "uuid")
      break;

    temp.push_back (*arg);
  }

  // Now insert the new sequence expression.
  temp.push_back (std::make_pair (sequence.str (), "exp"));

  // Now copy everything after the last id/uuid.
  bool found_id = false;
  for (arg = _args.begin (); arg != _args.end (); ++arg)
  {
    if (arg->second == "id" || arg->second == "uuid")
      found_id = true;

    else if (found_id)
      temp.push_back (*arg);
  }

  _args.swap (temp);
  _args.dump ("Expression::expand_sequence");
}

////////////////////////////////////////////////////////////////////////////////
// Inserts the 'and' operator by default between terms that are not separated by
// at least one operator.
//
// Converts:  <term1>     <term2> <op> <term>
// to:        <term1> and <term2> <op> <term>
//
void Expression::implicit_and ()
{
  Arguments temp;
  bool delta = false;

  std::string previous = "op";
  std::vector <std::pair <std::string, std::string> >::iterator arg;
  for (arg = _args.begin (); arg != _args.end (); ++arg)
  {
    // Old-style filters need 'and' conjunctions.
    if (previous    != "op" &&
        arg->second != "op")
    {
      temp.push_back (std::make_pair ("and", "op"));
      delta = true;
    }

    // Now insert the adjacent non-operator.
    temp.push_back (*arg);
    previous = arg->second;
  }

  if (delta)
  {
    _args.swap (temp);
    _args.dump ("Expression::implicit_and");
  }
}

////////////////////////////////////////////////////////////////////////////////
// Convert:  +with -without
// To:       tags ~ with
//           tags !~ without
void Expression::expand_tag ()
{
  Arguments temp;
  bool delta = false;

  std::vector <std::pair <std::string, std::string> >::iterator arg;
  for (arg = _args.begin (); arg != _args.end (); ++arg)
  {
    if (arg->second == "tag")
    {
      char type;
      std::string value;
      Arguments::extract_tag (arg->first, type, value);

      temp.push_back (std::make_pair ("tags", "lvalue"));
      temp.push_back (std::make_pair (type == '+' ? "~" : "!~", "op"));
      temp.push_back (std::make_pair (value, "rvalue"));
      delta = true;
    }
    else
      temp.push_back (*arg);
  }

  if (delta)
  {
    _args.swap (temp);
    _args.dump ("Expression::expand_tag");
  }
}

////////////////////////////////////////////////////////////////////////////////
// Convert:  /foo/
// To:       description ~ foo
void Expression::expand_pattern ()
{
  Arguments temp;
  bool delta = false;

  std::vector <std::pair <std::string, std::string> >::iterator arg;
  for (arg = _args.begin (); arg != _args.end (); ++arg)
  {
    if (arg->second == "pattern")
    {
      std::string value;
      Arguments::extract_pattern (arg->first, value);

      temp.push_back (std::make_pair ("description", "lvalue"));
      temp.push_back (std::make_pair ("~", "op"));
      temp.push_back (std::make_pair (value, "rvalue"));
      delta = true;
    }
    else
      temp.push_back (*arg);
  }

  if (delta)
  {
    _args.swap (temp);
    _args.dump ("Expression::expand_pattern");
  }
}

////////////////////////////////////////////////////////////////////////////////
// Convert:  <name>[:=]<value>
// To:       <name> = lex<value>
void Expression::expand_attr ()
{
  Arguments temp;
  bool delta = false;

  std::vector <std::pair <std::string, std::string> >::iterator arg;
  for (arg = _args.begin (); arg != _args.end (); ++arg)
  {
    if (arg->second == "attr")
    {
      // TODO Canonicalize 'name'.
      std::string name;
      std::string value;
      Arguments::extract_attr (arg->first, name, value);

      // Always quote the value, so that empty values, or values containing spaces
      // are preserved.
      value = "\"" + value + "\"";

      temp.push_back (std::make_pair (name, "lvalue"));
      temp.push_back (std::make_pair ("=", "op"));
      temp.push_back (std::make_pair (value, "rvalue"));
      delta = true;
    }
    else
      temp.push_back (*arg);
  }

  if (delta)
  {
    _args.swap (temp);
    _args.dump ("Expression::expand_attr");
  }
}

////////////////////////////////////////////////////////////////////////////////
// Convert:  <name>.<mod>[:=]<value>
// To:       <name> <op> lex<value>
void Expression::expand_attmod ()
{
  Arguments temp;
  bool delta = false;

  std::vector <std::pair <std::string, std::string> >::iterator arg;
  for (arg = _args.begin (); arg != _args.end (); ++arg)
  {
    if (arg->second == "attmod")
    {
      // TODO Should canonicalize 'name'.
      std::string name;
      // TODO Should canonicalize 'mod'.
      std::string mod;
      std::string value;
      std::string sense;
      Arguments::extract_attmod (arg->first, name, mod, value, sense);

      // Always quote the value, so that empty values, or values containing spaces
      // are preserved.
      std::string raw_value = value;
      value = "\"" + value + "\"";

      if (mod == "before" || mod == "under" || mod == "below")
      {
        temp.push_back (std::make_pair (name, "lvalue"));
        temp.push_back (std::make_pair ("<", "op"));
        temp.push_back (std::make_pair (value, "rvalue"));
      }
      else if (mod == "after" || mod == "over" || mod == "above")
      {
        temp.push_back (std::make_pair (name, "lvalue"));
        temp.push_back (std::make_pair (">", "op"));
        temp.push_back (std::make_pair (value, "rvalue"));
      }
      else if (mod == "none")
      {
        temp.push_back (std::make_pair (name, "lvalue"));
        temp.push_back (std::make_pair ("==", "op"));
        temp.push_back (std::make_pair ("\"\"", "rvalue"));
      }
      else if (mod == "any")
      {
        temp.push_back (std::make_pair (name, "lvalue"));
        temp.push_back (std::make_pair ("!=", "op"));
        temp.push_back (std::make_pair ("\"\"", "rvalue"));
      }
      else if (mod == "is" || mod == "equals")
      {
        temp.push_back (std::make_pair (name, "lvalue"));
        temp.push_back (std::make_pair ("=", "op"));
        temp.push_back (std::make_pair (value, "rvalue"));
      }
      else if (mod == "isnt" || mod == "not")
      {
        temp.push_back (std::make_pair (name, "lvalue"));
        temp.push_back (std::make_pair ("!=", "op"));
        temp.push_back (std::make_pair (value, "rvalue"));
      }
      else if (mod == "has" || mod == "contains")
      {
        temp.push_back (std::make_pair (name, "lvalue"));
        temp.push_back (std::make_pair ("~", "op"));
        temp.push_back (std::make_pair (value, "rvalue"));
      }
      else if (mod == "hasnt")
      {
        temp.push_back (std::make_pair (name, "lvalue"));
        temp.push_back (std::make_pair ("!~", "op"));
        temp.push_back (std::make_pair (value, "rvalue"));
      }
      else if (mod == "startswith" || mod == "left")
      {
        temp.push_back (std::make_pair (name, "lvalue"));
        temp.push_back (std::make_pair ("~", "op"));
        temp.push_back (std::make_pair ("^" + raw_value, "rx"));
      }
      else if (mod == "endswith" || mod == "right")
      {
        temp.push_back (std::make_pair (name, "lvalue"));
        temp.push_back (std::make_pair ("~", "op"));
        temp.push_back (std::make_pair (raw_value + "$", "rx"));
      }
      else if (mod == "word")
      {
        temp.push_back (std::make_pair (name, "lvalue"));
        temp.push_back (std::make_pair ("~", "op"));
        temp.push_back (std::make_pair ("\\b" + raw_value + "\\b", "rx"));
      }
      else if (mod == "noword")
      {
        temp.push_back (std::make_pair (name, "lvalue"));
        temp.push_back (std::make_pair ("!~", "op"));
        temp.push_back (std::make_pair ("\\b" + raw_value + "\\b", "rx"));
      }
      else
        throw std::string ("Error: unrecognized attribute modifier '") + mod + "'.";

      delta = true;
    }
    else
      temp.push_back (*arg);
  }

  if (delta)
  {
    _args.swap (temp);
    _args.dump ("Expression::expand_attmod");
  }
}

////////////////////////////////////////////////////////////////////////////////
// Convert:  <word>
// To:       description ~ <word>
void Expression::expand_word ()
{
  Arguments temp;
  bool delta = false;

  std::vector <std::pair <std::string, std::string> >::iterator arg;
  for (arg = _args.begin (); arg != _args.end (); ++arg)
  {
    if (arg->second == "word")
    {
      temp.push_back (std::make_pair ("description", "lvalue"));
      temp.push_back (std::make_pair ("~", "op"));
      temp.push_back (std::make_pair (arg->first, "rvalue"));

      delta = true;
    }
    else
      temp.push_back (*arg);
  }

  if (delta)
  {
    _args.swap (temp);
    _args.dump ("Expression::expand_word");
  }
}

////////////////////////////////////////////////////////////////////////////////
// Look for "exp" arguments, and convert them to one of:
//   "date"
//   "duration"
//   Lexer::tokenize
void Expression::expand_expression ()
{
  Arguments temp;
  bool delta = false;

  // Get a list of all operators.
  std::vector <std::string> operators = Arguments::operator_list ();

  // Look for all 'exp' args.
  std::vector <std::pair <std::string, std::string> >::iterator arg;
  for (arg = _args.begin (); arg != _args.end (); ++arg)
  {
    if (arg->second == "exp")
    {
      // Remove quotes.
      arg->first = unquoteText (arg->first);

      if (Date::valid (arg->first, context.config.get ("dateformat")))
        temp.push_back (std::make_pair (arg->first, "date"));

      else if (Duration::valid (arg->first))
        temp.push_back (std::make_pair (arg->first, "duration"));

      // The expression does not appear to be syntactic sugar, so it should be
      // lexed.
      else
      {
        Lexer lexer (arg->first);
        lexer.skipWhitespace (true);
        lexer.coalesceAlpha (true);
        lexer.coalesceDigits (true);
        lexer.coalesceQuoted (true);

        std::vector <std::string> tokens;
        lexer.tokenize (tokens);

        std::vector <std::string>::iterator token;
        for (token = tokens.begin (); token != tokens.end (); ++token)
          temp.push_back (
            std::make_pair (
              *token,
              (_args.is_operator (*token) ? "op" : "dom")));
      }

      delta = true;
    }
    else
      temp.push_back (*arg);
  }

  if (delta)
  {
    _args.swap (temp);
    _args.dump ("Expression::expand_expression");
  }
}

////////////////////////////////////////////////////////////////////////////////
// Dijkstra Shunting Algorithm.
// http://en.wikipedia.org/wiki/Shunting-yard_algorithm
//
//   While there are tokens to be read:
//     Read a token.
//     If the token is an operator, o1, then:
//       while there is an operator token, o2, at the top of the stack, and
//             either o1 is left-associative and its precedence is less than or
//             equal to that of o2,
//             or o1 is right-associative and its precedence is less than that
//             of o2,
//         pop o2 off the stack, onto the output queue;
//       push o1 onto the stack.
//     If the token is a left parenthesis, then push it onto the stack.
//     If the token is a right parenthesis:
//       Until the token at the top of the stack is a left parenthesis, pop
//       operators off the stack onto the output queue.
//       Pop the left parenthesis from the stack, but not onto the output queue.
//       If the token at the top of the stack is a function token, pop it onto
//       the output queue.
//       If the stack runs out without finding a left parenthesis, then there
//       are mismatched parentheses.
//     If the token is a number, then add it to the output queue.
//
//   When there are no more tokens to read:
//     While there are still operator tokens in the stack:
//       If the operator token on the top of the stack is a parenthesis, then
//       there are mismatched parentheses.
//       Pop the operator onto the output queue.
//   Exit.
//
void Expression::postfix ()
{
  Arguments temp;

  std::pair <std::string, std::string> item;
  Arguments op_stack;
  char type;
  int precedence;
  char associativity;

  std::vector <std::pair <std::string, std::string> >::iterator arg;
  for (arg = _args.begin (); arg != _args.end (); ++arg)
  {
    if (arg->first == "(")
    {
      op_stack.push_back (*arg);
    }
    else if (arg->first == ")")
    {
      while (op_stack.size () > 0 &&
             op_stack.back ().first != "(")
      {
        temp.push_back (op_stack.back ());
        op_stack.pop_back ();
      }

      if (op_stack.size ())
      {
        op_stack.pop_back ();
      }
      else
      {
        throw std::string ("Mismatched parentheses in expression");
      }
    }
    else if (Arguments::is_operator (arg->first, type, precedence, associativity))
    {
      char type2;
      int precedence2;
      char associativity2;
      while (op_stack.size () > 0 &&
             Arguments::is_operator (op_stack.back ().first, type2, precedence2, associativity2) &&
             ((associativity == 'l' && precedence <= precedence2) ||
              (associativity == 'r' && precedence <  precedence2)))
      {
        temp.push_back (op_stack.back ());
        op_stack.pop_back ();
      }

      op_stack.push_back (*arg);
    }
    else
    {
      temp.push_back (*arg);
    }
  }

  while (op_stack.size () != 0)
  {
    if (op_stack.back ().first == "(" ||
        op_stack.back ().first == ")")
      throw std::string ("Mismatched parentheses in expression");

    temp.push_back (op_stack.back ());
    op_stack.pop_back ();
  }

  _args.swap (temp);
  _args.dump ("Expression::toPostfix");
}

////////////////////////////////////////////////////////////////////////////////