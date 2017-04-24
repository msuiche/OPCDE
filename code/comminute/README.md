
# Comminute

The code here is intended to be some basics that help with learning
the LLVM API. The points dealt with here are:

- Pass manager use
- Pass dependency
- Some basic IR instruction and value analysis
- Use/User API 

It is not meant to be a some great bug hunting tool. It is meant
to help get you to the point where you can start to think about 
interprocedural and other analyses. Once you get the feel you
should start to look at other code, like SVF, to get into things.
You should look at building CFGs so you can evaluate globals
better. You should look at some extended interprocedural SSA or
perhaps using Andersen's Alias Analysis for analyzing pointers.
There is a lot to doing good static analysis and that's where the
meat of the research is!

I repeat... Just a learning tool.
