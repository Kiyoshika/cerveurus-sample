#include "HTTP_Server.h"
#include "Person.h"

int main()
{
	HTTP_Server http_server;
	http_init(&http_server, 7000);

	// setup routes for Person struct
	struct PersonRepository people;
	person_init(&people, 1);

	// add routes manage the "people" struct so we pass it by address and its deallocator
	http_add_route_GET(&http_server, "/api/v1/person/getAll", &people, &person_free, &person_get_all);
	http_add_route_POST(&http_server, "/api/v1/person/add", &people, &person_free, &person_add);
	http_add_route_DELETE(&http_server, "/api/v1/person/delete", &people, &person_free, &person_delete);

	// start main loop
	http_listen(&http_server);
	return 0;
}
