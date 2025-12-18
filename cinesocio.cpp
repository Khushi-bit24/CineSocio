#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <iomanip>
using namespace std;
class User{
private:
string name;
string password;
bool isLoggedIn = false;
vector<User *> friends;
vector<User *> friendRequests;
public:
User(string name, string password) : name(name), password(password) {}
bool login(string inputPassword) {
if (inputPassword == password){
isLoggedIn = true;
return true;
}
return false;
}
bool isAuthenticated() { return isLoggedIn; }
string getName() { return name; }
void sendFriendRequest(User &user){
if (this == &user) {
cout << "You cannot send a friend request to yourself!\n";
return;
}
if (find(user.friendRequests.begin(), user.friendRequests.end(), this) == user.friendRequests.end() 
&&
find(friends.begin(), friends.end(), &user) == friends.end()){
user.friendRequests.push_back(this);
cout << "Friend request sent to " << user.getName() << "!\n";
}
else{
cout << "Friend request already sent or already friends!\n";
}
}
void acceptFriendRequest(User &user){
if (friendRequests.empty()){
cout << "You have no friend requests to accept!\n";
return;
}
auto it = find(friendRequests.begin(), friendRequests.end(), &user);
if (it != friendRequests.end()){
friends.push_back(&user);
user.friends.push_back(this);
friendRequests.erase(it);
cout << "Friend request from " << user.getName() << " accepted!\n";
}
else{
cout << "No friend request found from " << user.getName() << "!\n";
}
}
void declineFriendRequest(User &user) {
if (friendRequests.empty()){
cout << "You have no friend requests to decline!\n";
return;
}
auto it = find(friendRequests.begin(), friendRequests.end(), &user);
if (it != friendRequests.end()){
friendRequests.erase(it);
cout << "Friend request from " << user.getName() << " declined!\n";
}
else{
cout << "No friend request found from " << user.getName() << "!\n";
}
}
vector<User *> &getFriendRequests() { return friendRequests; }
};
class Post{
public:
string content;
int likes;
int dislikes;
string owner;
vector<string> usersLiked;
vector<string> usersDisliked;
Post(string content, string owner) : content(content), likes(0), dislikes(0), owner(owner) {}
void like(string user){
if (find(usersLiked.begin(), usersLiked.end(), user) != usersLiked.end()){
cout << "You have already liked this post!\n";
return;
}
if(find(usersDisliked.begin(), usersDisliked.end(), user) != usersDisliked.end()){
dislikes--;
usersDisliked.erase(remove(usersDisliked.begin(), usersDisliked.end(), user), 
usersDisliked.end());
}
likes++;
usersLiked.push_back(user);
}
void dislike(string user){
if (find(usersDisliked.begin(), usersDisliked.end(), user) != usersDisliked.end()){
cout << "You have already disliked this post!\n";
return;
}
if (find(usersLiked.begin(), usersLiked.end(), user) != usersLiked.end()){
likes--;
usersLiked.erase(remove(usersLiked.begin(), usersLiked.end(), user), usersLiked.end());
}
dislikes++;
usersDisliked.push_back(user);
}
void displayPost(int index){
cout << "[" << index << "] " << owner << " posted: " << content << " | Likes: " << likes << " | Dislikes: " << dislikes << endl;
}
};
class Insta{
vector<User *> users;
vector<Post> posts;
public:
~Insta(){
for (User *user : users){
delete user;
}
}
void signInMenu(){
int choice;
while (true){
cout << "1. Sign Up\n2. Login\n3. Exit\nEnter your choice: ";
cin >> choice;
if (choice == 1) {
signUp();
}
else if (choice == 2){
User *loggedInUser = login();
if (loggedInUser){
displayMenu(*loggedInUser);
}
}
else{
cout << "Exiting Insta...\n";
break;
}
}
}
void signUp(){
string username, password;
cout << "Enter username: ";
cin >> username;
cout << "Enter password: ";
cin >> password;
users.push_back(new User(username, password));
cout << "Sign up successful!\n";
}
User *login(){
string username, password;
cout << "Enter username: ";
cin >> username;
cout << "Enter password: ";
cin >> password;
for (User *user : users){
if (user->getName() == username && user->login(password)) {
cout << "Login successful!\n";
return user;
}
}
cout << "Invalid credentials.\n";
return nullptr;
}
void displayMenu(User &user){
int choice;
while (true){
cout << "1. Add Post\n"
     << "2. Like Post\n"
     << "3. Dislike Post\n"
     << "4. View Posts\n"
     << "5. Send Friend Request\n"
     << "6. Accept Friend Request\n"
     << "7. Decline Friend Request\n"
     << "8. Logout\n"
     << "Enter choice: ";

cin >> choice;
if (choice == 1){
string content;
cout << "Enter post content: ";
cin.ignore();
getline(cin, content);
posts.emplace_back(content, user.getName());
cout << "Post added successfully!\n";
}
else if (choice == 2 || choice == 3) {
int index;
cout << "Enter post index: ";
cin >> index;
if (index >= 0 && index < posts.size()){
if (choice == 2)
posts[index].like(user.getName());
else
posts[index].dislike(user.getName());
}
else{
cout << "Invalid post index!\n";
}
}
else if (choice == 4){
if (posts.empty()){
cout << "No posts available!\n";
}
else{
for (size_t i = 0; i < posts.size(); i++){
posts[i].displayPost(i);
}
}
}
else if (choice == 5){
string friendName;
cout << "Enter friend's username: ";
cin >> friendName;
bool found = false;
for (User *u : users){
if (u->getName() == friendName){
user.sendFriendRequest(*u);
found = true;
break;
}
}
if (!found)
cout << "User not found!\n";
}
else if (choice == 6) {
if (user.getFriendRequests().empty()){
cout << "No friend requests available!\n";
}
else{
cout << "Pending friend requests:\n";
for (size_t i = 0; i < user.getFriendRequests().size(); i++){
cout << i + 1 << ". " << user.getFriendRequests()[i]->getName() << endl;
}
int requestIndex;
cout << "Enter the number of the request to accept: ";
cin >> requestIndex;
if (requestIndex > 0 && requestIndex <= user.getFriendRequests().size()){
user.acceptFriendRequest(*user.getFriendRequests()[requestIndex - 1]);
}
else{
cout << "Invalid selection!\n";
}
}
}
else if (choice == 7){
if (user.getFriendRequests().empty()){
cout << "No friend requests available!\n";
}
else{
cout << "Pending friend requests:\n";
for (size_t i = 0; i < user.getFriendRequests().size(); i++){
cout << i + 1 << ". " << user.getFriendRequests()[i]->getName() << endl;
}
int requestIndex;
cout << "Enter the number of the request to decline: ";
cin >> requestIndex;
if (requestIndex > 0 && requestIndex <= user.getFriendRequests().size()) {
user.declineFriendRequest(*user.getFriendRequests()[requestIndex - 1]);
}
else{
cout << "Invalid selection!\n";
}
}
}
else if (choice == 8){
cout << "Logging out...\n";
break;
}
else{
cout << "Invalid choice! Please enter a valid option.\n";
}
}
}
};
class Netflix{
vector<User *> users;
vector<string> availableLanguages = {"English", "Hindi", "Tamil", "Telugu", "Korean"};
vector<string> availableGenres = {
"Action", "Drama", "Comedy", "Thriller", "Mystery",
"Romance", "Sci-Fi"};
struct Movie {
string title;
string genre;
vector<string> languages;
};
vector<Movie> movies = {
{"Vikram Vedha", "Thriller", {"Tamil", "Hindi"}},
{"Drishyam", "Thriller", {"Hindi", "Telugu"}},
{"Parasite", "Thriller", {"Korean", "English"}},
{"Titanic", "Drama", {"English"}},
{"The Dark Knight", "Action", {"English", "Hindi"}},
{"Bahubali", "Action", {"Telugu", "Hindi", "Tamil"}},
{"96", "Romance", {"Tamil"}},
{"Inception", "Sci-Fi", {"English", "Hindi"}},
{"Talaash", "Thriller", {"Hindi"}},
{"Raat Akeli Hai", "Thriller", {"Hindi"}}};
public:
~Netflix(){
for (User *user : users)
delete user;
}
void signInMenu(){
int choice;
while (true) {
cout << "\n1. Sign Up\n2. Login\n3. Exit Netflix\nEnter your choice: ";
cin >> choice;
if (choice == 1){
signUp();
}
else if (choice == 2){
User *loggedInUser = login();
if (loggedInUser)
displayMenu(*loggedInUser);
}
else if (choice == 3){
cout << "Exiting Netflix...\n";
break;
}
else{
cout << "Invalid choice! Please try again.\n";
}
}
}
private:
void signUp(){
string username, password;
cout << "Enter username: ";
cin >> username;
cout << "Enter password: ";
cin >> password;
users.push_back(new User(username, password));
cout << "Sign up successful!\n";
}
User *login(){
string username, password;
cout << "Enter username: ";
cin >> username;
cout << "Enter password: ";
cin >> password;
for (User *user : users) {
if (user->getName() == username && user->login(password)) {
cout << "Login successful!\n";
return user;
}
}
cout << "Invalid credentials.\n";
return nullptr;
}
void displayMenu(User &user){
int choice;
while (true){
cout << "\n****** NETFLIX MENU ******\n";
cout << "1. Choose Language, Genre, Movies/Shows\n";
cout << "2. Search Movie\n";
cout << "3. Logout\n";
cout << "Enter your choice: ";
cin >> choice;
switch (choice){
case 1:
chooseMovies();
break;
case 2:
searchMovie();
break;
case 3:
cout << "Logging out of Netflix...\n";
return;
default:
cout << "Invalid choice! Please try again.\n";
}
}
}
static string toLower(const string &str){
string res = str;
transform(res.begin(), res.end(), res.begin(), ::tolower);
return res;
}
void chooseMovies(){
int langChoice, genreChoice;
cout << "\nAvailable Languages:\n";
for (size_t i = 0; i < availableLanguages.size(); i++)
cout << i + 1 << ". " << availableLanguages[i] << endl;
cout << "Choose language: ";
cin >> langChoice;
cout << "\nAvailable Genres:\n";
for (size_t i = 0; i < availableGenres.size(); i++)
cout << i + 1 << ". " << availableGenres[i] << endl;
cout << "Choose genre: ";
cin >> genreChoice;
if (langChoice < 1 || langChoice > availableLanguages.size() ||
genreChoice < 1 || genreChoice > availableGenres.size()){
cout << "Invalid choice!\n";
return;
}
string selectedLang = toLower(availableLanguages[langChoice - 1]);
string selectedGenre = toLower(availableGenres[genreChoice - 1]);
cout << "\nMovies/Shows in " << availableLanguages[langChoice - 1]
<< " and " << availableGenres[genreChoice - 1] << ":\n";
bool found = false;
for (const Movie &movie : movies){
if (toLower(movie.genre) == selectedGenre) {
for (const string &lang : movie.languages){
if (toLower(lang) == selectedLang){
cout << "- " << movie.title << endl;
found = true;
break;
}
}
}
}
if (!found)
cout << "No movies found in the selected language and genre.\n";
}
void searchMovie()
{
if (movies.empty()){
cout << "No movies available.\n";
return;
}
cin.ignore();
cout << "Enter the movie title you want to search: ";
string searchTitle;
getline(cin, searchTitle);
searchTitle = toLower(searchTitle);
bool found = false;
int index = -1;
for (size_t i = 0; i < movies.size(); ++i){
if (toLower(movies[i].title) == searchTitle){
found = true;
index = i;
break;
}
}
if (!found){
cout << "Movie not found.\n";
return;
}
cout << "Movie found: " << movies[index].title << endl;
cout << "Do you want to:\n1. Watch Trailer\n2. Watch Movie\nEnter your choice: ";
int choice;
cin >> choice;
cin.ignore();
cout << "Enter language: ";
string lang;
getline(cin, lang);
lang = toLower(lang);
bool languageAvailable = false;
for (auto &availableLang : movies[index].languages){
if (toLower(availableLang) == lang){
languageAvailable = true;
break;
}
}
if (!languageAvailable){
cout << "Requested language not available. Available languages are: ";
for (auto &l : movies[index].languages){
cout << l << " ";
}
cout << endl;
return;
}
if (choice == 1){
cout << "Playing trailer for " << movies[index].title << " in " << lang << "...\n";
}
else if (choice == 2){
cout << "Streaming movie: " << movies[index].title << " in " << lang << "...\nEnjoy!\n";
}
else{
cout << "Invalid choice.\n";
}
}
};
class BookMyShow{
vector<User *> users;
struct Show{
string movieTitle;
vector<string> availableLocations;
map<string, int> seatsLeftPerLocation;
double ratingSum = 0;
int ratingCount = 0;
};
struct Booking{
string movieTitle;
string location;
int seatsBooked;
};
map<string, vector<Booking>> userBookings;
vector<Show> shows = {
{"Inception", {"Miraj Cinema", "PVR", "INOX", "Cinepolis"}, {{"Miraj Cinema", 50}, {"PVR", 50}, 
{"INOX", 50}, {"Cinepolis", 50}}},
{"Bahubali", {"PVR", "Cinepolis", "Miraj Cinema"}, {{"PVR", 50}, {"Cinepolis", 50}, {"Miraj Cinema", 50}}},
{"Jawan", {"Miraj Cinema", "Cinepolis", "PVR"}, {{"Miraj Cinema", 50}, {"Cinepolis", 50}, {"PVR", 
50}}}};
public:
~BookMyShow(){
for (auto *u : users)
delete u;
}
void signInMenu(){
int choice;
while (true){
cout << "\n*** Welcome to BookMyShow ***\n"
<< "1. Sign Up\n2. Login\n3. Exit BookMyShow\nEnter your choice: ";
cin >> choice;
if (choice == 1)
signUp();
else if (choice == 2){
if (User *u = login())
displayMenu(*u);
}
else if (choice == 3){
cout << "Exiting BookMyShow...\n";
break;
}
else
cout << "Invalid choice! Try again.\n";
}
}
private:
void signUp(){
string user, pwd;
cout << "Enter username: ";
cin >> user;
cout << "Enter password: ";
cin >> pwd;
users.push_back(new User(user, pwd));
cout << "Sign up successful!\n";
}
User *login(){
string user, pwd;
cout << "Enter username: ";
cin >> user;
cout << "Enter password: ";
cin >> pwd;
for (auto *u : users)
if (u->getName() == user && u->login(pwd)){
cout << "Login successful!\n";
return u;
}
cout << "Invalid credentials.\n";
return nullptr;
}
void displayMenu(User &me){
int choice;
while (true){
cout << "\n*** BOOKMYSHOW MENU ***\n"
<< "1. Check Movie Availability\n"
<< "2. Book Tickets\n"
<< "3. Rate Movie\n"
<< "4. Logout\nEnter your choice: ";
cin >> choice;
switch (choice){
case 1:
listShows();
break;
case 2:
bookMovie(me);
break;
case 3:
rateMovie();
break;
case 4:
cout << "Logging out...\n";
return;
default:
cout << "Invalid choice!\n";
}
}
}
void listShows(){
cout << "\nAvailable Movies:\n";
for (auto &s : shows){
cout << "- " << s.movieTitle << " [Locations: ";
for (auto &loc : s.availableLocations)
cout << loc << ", ";
cout << "]\n";
}
}
void bookMovie(User &me){
cin.ignore(numeric_limits<streamsize>::max(), '\n');
string movieName;
string location;
int seats;
cout << "Enter movie name: ";
getline(cin, movieName);
Show *s = findShow(movieName);
if (!s) {
cout << "Movie not found.\n";
return;
}
cout << "Available locations: ";
for (size_t i = 0; i < s->availableLocations.size(); ++i){
cout << s->availableLocations[i];
if (i + 1 < s->availableLocations.size())
cout << ", ";
}
cout << "\nChoose location: ";
getline(cin, location);
string locKey;
for (auto &loc : s->availableLocations){
if (toLower(loc) == toLower(location)){
locKey = loc;
break;
}
}
if (locKey.empty()){
cout << "Location not available for this movie.\n";
return;
}
int avail = s->seatsLeftPerLocation[locKey];
cout << "Seats available: " << avail << "\n"
<< "Enter number of seats to book: ";
cin >> seats;
if (seats <= 0 || seats > avail){
cout << "Not enough seats available! Try fewer seats.\n";
return;
}
s->seatsLeftPerLocation[locKey] -= seats;
userBookings[me.getName()].push_back({s->movieTitle, locKey, seats});
cout << "Booking successful! " << seats << " seats booked for "
<< s->movieTitle << " at " << locKey << ".\n";
}
void rateMovie(){
cin.ignore();
cout << "Enter the movie you want to rate: ";
string movie;
getline(cin, movie);
Show *s = findShow(movie);
if (!s){
cout << "Movie not found.\n";
return;
}
cout << "Enter rating (1 to 5): ";
int r;
cin >> r;
if (r < 1 || r > 5){
cout << "Invalid rating.\n";
return;
}
s->ratingSum += r;
s->ratingCount++;
cout << "Thank you for rating! Current Average: "
<< fixed << setprecision(1)
<< (s->ratingSum / s->ratingCount) << "/5\n";
}
Show *findShow(const string &title){
for (auto &s : shows)
if (toLower(s.movieTitle) == toLower(title))
return &s;
return nullptr;
}
static string toLower(const string &in){
string out = in;
transform(out.begin(), out.end(), out.begin(), ::tolower);
return out;
}
};
int main(){
int choice;
cout << "WELCOME TO CINESOCIO APPS" << endl;
while (true) {
cout << "MENU \n1.INSTA \n2.NETFLIX \n3.BOOK MY SHOW\n4.EXIT CINESOCIO APPS\n";
cout << "ENTER YOUR CHOICE" << endl;
cin >> choice;
if (choice == 1){
Insta insta;
cout << "******INSTA******" << endl;
insta.signInMenu();
}
else if (choice == 2){
Netflix netflix;
cout << "******NETFLIX******" << endl;
netflix.signInMenu();
}
else if (choice == 3){
BookMyShow bms;
cout << "******BOOK MY SHOW******" << endl;
bms.signInMenu();
}
else if (choice == 4){
cout << "***EXITING***" << endl;
break;
}
else{
cout << "Invalid choice , please enter a valid choice from menu" << endl;
 } 
} 

return 0;
}