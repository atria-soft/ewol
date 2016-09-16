Ewol coding style                       {#ewol_coding_style}
=================

@tableofcontents


Comments                                {#ewol_coding_style_comment}
========

- One line comment: (never in #define xxx ==> too dangerous)
```{.cpp}
//
```

- Multiple line comment
```{.cpp}
/*
 * xxxx yyyy
 * zzzz
 */
```

- Documentation : doxygen (do not set un-nneded field)

@verbatim
/**
 * @brief my summery
 * @param[in,out] _xxxx Comment on the variable
 * @return my return explanation
 */
@endverbatim

- one line documlentation:
```{.cpp}
xxxxx, //!< my comment
```




Number of colomn                                {#ewol_coding_style_number_of_colomn}
================

I do not linit the number of colomn, but it could be good to limit at 150 char. Many screen have the main display like this



Indentation & braces                                {#ewol_coding_style_indent}
====================

Tab size is a personal choice then, when you write code, you might be
tab proof. If someone want to have the golden number for theire tabs,
he will be able to do it.
When you set a brace '{' you need to add a brace, and when you set a
stop brace '}' you need to remove a tab
To be simple : (tab stop at the 'if' start)

- if:
```{.cpp}
	if (    xxx == yyy
	     && (    xxx == kkk
	          || xxx == zzz )) {
		//your action ...;
	}
```

- switch:
```{.cpp}
	switch (suffix) {
		case 'G':
		case 'g':
			mem <<= 30;
			break;
		case 'M':
		case 'm':
			mem <<= 20;
			break;
		case 'K':
		case 'k':
			mem <<= 10;
			// fall through
		default:
			break;
	}
```

- function:
```{.cpp}
	void myFunction(void) {
		actions ...;
	}
```

- classes:
```{.cpp}
	class MyClass {
		public:
			MyClass(void);
			~MyClass(void);
		private:
			const char* getName(void);
	};
```

- namespace:
```{.cpp}
	namespace appl {
		void get(void);
	}
```

- For special element like : you might add a tabulation too
```{.cpp}
	case xxx:
		actions...
	public:
		definition ...
```

- An exception for the inline function inside c++ header:
```{.cpp}
	class Plop {
		private:
			int32_t m_value; //!< my value of money gain
		public:
			int32_t getValue(void) const { return m_value; };
```



Types                                {#ewol_coding_style_type}
=====

the element 'typedef' must not be use, like this it is not needed to add
special element like '_te' or '_ts' to say respectively 'tpedef enum' and
'typedef struct'
Structure is not availlable in c++, just use normal class, this is the same.



Star position                                {#ewol_coding_style_star}
=============

The star will be near the type :
```{.cpp}
	void* myVariableName;
```



C and c++                                {#ewol_coding_style_c_and_cpp}
=========

All C header files might have :
```{.cpp}
#ifdef __cplusplus
	extern "C" {
#endif

...

#ifdef __cplusplus
	}
#endif
```




Naming                                {#ewol_coding_style_naming}
======

- Fonction/Methods:
Camel case with first letter in lower case.
```{.cpp}
void myExampleFontionName(void);
```

- Variable:
Camel case with first letter in lower case.
```{.cpp}
nt32_t myVariableExample;
```

- namespace:
one world in lower case
```{.cpp}
namspace ewol {
```

- Class:
Camel case with first letter in upper case.
```{.cpp}
class MyClass;
```

- Members fields:
Put a 'm' prefix and then a normal Variable name
```{.cpp}
int32_t m_memberField;
```

- enum:
Camel case with first letter in lower case.
```{.cpp}
enum myEnum {
	myEnum_def1,
	myEnum_def2,
	myEnum_def3,
};
```
- structure (C only)
use naming like Classes (and for mamber too)

- minimum size : Do not use variable with size <3, the for a iterator 
for a 'for' :
```{.cpp}
for (int32_t iii=0; iii<xxx; ++iii) {
	// actions ...
}
```




Types                                {#ewol_coding_style_types}
=====

Une stanndard Type :
```{.cpp}
		bool
		int8_t / uint8_t
		int16_t / uint16_t
		int32_t / uint32_t
		int64_t / uint64_t
		size_t
		int (some case needed)
		float / double
		float_t to automatic match whith the compilation choice between float or double
```




C++ specificity                                {#ewol_coding_style_specific}
===============

- STL
	You can use the Stl, but the porting and the result can be
	different depending on the board you are.
- Heritage:
	Simple Heritage is a good use of the C++, multiple heritage is
	not really supported in every compilators.
- const:
	Set the parameter and fonction const all time you can.
- exception:
	They are not manage in all platforms, then by restriction, do
	not use it.

