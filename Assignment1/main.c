// Problem domain: Storing UFC fighter and event records. Events could include venue, date, gate, box office,
// main event, etc. Fighter records could include name, weight classes, championships, age, ranking, and more.
// 
// record types: UFC Fighters (alphabetically sorted names) and UFC event codes (UFC203, UFC100, etc.) These are
// sorted numerically and the 'UFC' at the beginning is ignored (as every record includes this).
// 
// Most time sensitive operation: ?
// 
// TO-DO: 
// Implement data structures to store identifiers
// Implement a 1:1 relationship
// Implement M:M relationship
//

/* NOTES
There will be more fighter records than event records. There can be over 30 fighters in a single event and careers
tend to be short, so of course there must be multiple times more fighters than events. 

Events would be a perfect array. The event codes (E.g., UFC203) could easily be accessed as an array index. As
the project only is dealing with UFC events, there will never be over than 1 record at index 203. UFC events
are numbered in incremental numerical order, so there wil never be an insertion at the front or middle, thus
no shuffling. However, an array would limit the ability to insert new events. If I am able for the purposes of this
assignment make the database only store a certain number of UFC events (today at the time of writing is UFC314, so 
that would be a good choice) then this is feasible, but if the database needs to handle insertion of new events then
an array is unfortunately not feasible despite being otherwise perfect.

There are thousands of past UFC fighters. An array of them would be too large and again have the same issue of 
limiting insertions due to its fixed size. A 

*/

int main() {
 
	return 0;
}