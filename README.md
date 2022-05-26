## [Get this title for $10 on Packt's Spring Sale](https://www.packt.com/B09734?utm_source=github&utm_medium=packt-github-repo&utm_campaign=spring_10_dollar_2022)
-----
For a limited period, all eBooks and Videos are only $10. All the practical content you need \- by developers, for developers

# Hands-On Design Patterns with C++

<a href="https://www.packtpub.com/application-development/hands-design-patterns-c?utm_source=github&utm_medium=repository&utm_campaign=9781788832564 "><img src="https://d255esdrn735hr.cloudfront.net/sites/default/files/imagecache/ppv4_main_book_cover/B09734_NEW.png" alt="Hands-On Design Patterns with C++" height="256px" align="right"></a>

This is the code repository for [Hands-On Design Patterns with C++](https://www.packtpub.com/application-development/hands-design-patterns-c?utm_source=github&utm_medium=repository&utm_campaign=9781788832564 ), published by Packt.

**Solve common C++ problems with modern design patterns and build robust applications**

## What is this book about?
C++ is a general-purpose programming language built with a bias towards embedded programming and systems programming. Design patterns are, in essence, a library of reusable components, only for software architecture, not for a concrete implementation.

This book covers the following exciting features:
* Recognize some of the most common design patterns used in C++, and what additional information is expressed by their use in context. 
* Understand how to use C++ generic programming to solve some of the most common design problems. 
* Explore the most powerful C++ idioms, their strengths, and drawbacks. 
* Rediscover some of the most popular C++ idioms, such as Resource Acquisition Is Initialization, and learn the new ways they can be used together with generic programming. 
* Understand the impact of different design and implementation choices on the program performance, and how to write well-designed and readable programs without sacrificing performance. 

If you feel this book is for you, get your [copy](https://www.amazon.com/dp/1788832566) today!

<a href="https://www.packtpub.com/?utm_source=github&utm_medium=banner&utm_campaign=GitHubBanner"><img src="https://raw.githubusercontent.com/PacktPublishing/GitHub/master/GitHub.png" 
alt="https://www.packtpub.com/" border="5" /></a>

## Instructions and Navigations
All of the code is organized into folders. For example, Chapter02.

The code will look like the following:
```
template <typename T> 
T increment(T x) { return x + 1; }
```

**Following is what you need for this book:**
This book is for experienced C++ developers and programmers who wish to learn about software design patterns and principles and apply them to create robust, reusable, and easily maintainable apps.

With the following software and hardware list you can run all code files present in the book (Chapter 1-18).
### Software and Hardware List
| Chapter | Software required | OS required |
| -------- | ------------------------------------ | ----------------------------------- |
| 5,6,8-11,15 | Google Benchmark | Windows, Mac OS X, and Linux  |
| 5 | Google Test | Windows, Mac OS X, and Linux  |
| 3,4 | Guidelines Support Library (GSL) | Windows, Mac OS X, and Linux |
| 11 | Folly | Windows, Mac OS X, and Linux  |

### Errata
* The code block at page 43 _should be_ as follows:
```
Group<int, long> g(3, 5);
int(g); // 3
long(g); // 5

```
* Page 429: The first code block on this page should be as follows:
```
void Point::accept(Visitor& v){
    v.visit(x_);    // double
    v.visit(y_);    // double
}

void Circle::accept(Visitor& v){
    v.visit(c_);    // Point
    v.visit(r_);    // double
}

void Line::accept(Visitor& v){
    v.visit(p1_);    // Point
    v.visit(p2_);    // Point
}
```


### Related products
* Hands-On System Programming with C++ [[Packt]](https://www.packtpub.com/application-development/hands-system-programming-c?utm_source=github&utm_medium=repository&utm_campaign=9781789137880 ) [[Amazon]](https://www.amazon.com/dp/1789137888)

* The Modern C++ Challenge [[Packt]](https://www.packtpub.com/application-development/modern-c-challenge?utm_source=github&utm_medium=repository&utm_campaign=9781788993869 ) [[Amazon]](https://www.amazon.com/dp/1788993861)

  
## Get to Know the Author
**Fedor G Pikus**
is a chief engineering scientist in the Design-to-Silicon division of Mentor Graphics (a Siemens business), and is responsible for the long-term technical direction of Calibre products, the design and architecture of software, and research into new software technologies. His earlier positions include senior software engineer at Google and chief software architect at Mentor Graphics. Fedor is a recognized expert on high-performance computing and C++. He has presented his works at CPPCon, SD West, DesignCon, and in software development journals, and is also an O'Reilly author. Fedor has over 25 patents, and over 100 papers and conference presentations on physics, EDA, software design, and C++.


### Suggestions and Feedback
[Click here](https://docs.google.com/forms/d/e/1FAIpQLSdy7dATC6QmEL81FIUuymZ0Wy9vH1jHkvpY57OiMeKGqib_Ow/viewform) if you have any feedback or suggestions.


