* The Object Oriented Database
The goal of this exercise is to create a database to store the contents of the employees.txt file. The obvious
method to do this is to create a container for the different staff types, and as this is an exercise in object
orientated development, I expect you to use an OO hierarchy with virtual functions etc. So as an example, you
might have a base class called "Staff", from which you derive "AdminStaff", "ProductionStaff" and "ResearchStaff".
Then you'll need to create some container to hold these pointers to "Staff" objects.

For your container, you might look into STL std::list (numerous other solutions exist, depending on your usage
patterns: feel free to make any assumptions you like. This container should live inside of the Database class.

Regular expression matching is used for tokenizing the input line - I intended to make different types of
staff have different fields which is easy to manage with additional regular expressions. However this over-
complicated the exercise.

Richard Southern
08/03/2017
