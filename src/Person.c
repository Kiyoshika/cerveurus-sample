#include "Person.h"
#include "cJSON.h"

#include "Routes.h" // struct CallbackArgs

#include <stdbool.h>

void person_init(
		struct PersonRepository* const people,
		size_t capacity)
{
	// assuming no allocation errors, #livingdangerously
	people->people = calloc(capacity, sizeof(struct Person));

	people->n_people = 0;
	people->capacity = capacity;
}

// return the response as following format
/*
 * {
 *     "people": [
 *         {
 *             "firstName": "Jimmy",
 *             "lastName": "John",
 *             "age": 25
 *         },
 *
 *         {
 *             "firstName": "Alex",
 *             "lastName": "Campbell",
 *             "age": 33
 *         }
 *     ]
 * }
 */
void person_get_all(
		struct CallbackArgs* const args)
{
	cJSON* body_json = cJSON_CreateObject();
	cJSON* people_array_json  = cJSON_AddArrayToObject(body_json, "people");

	struct PersonRepository* people = args->user_data;

	for (size_t i = 0; i < people->n_people; ++i)
	{
		cJSON* people_json = cJSON_CreateObject();

		cJSON_AddStringToObject(people_json, "firstName", people->people[i].firstName);
		cJSON_AddStringToObject(people_json, "lastName", people->people[i].lastName);
		cJSON_AddNumberToObject(people_json, "age", people->people[i].age);

		cJSON_AddItemToArray(people_array_json, people_json);
	}

	*(args->response_body) = cJSON_Print(body_json);
	cJSON_Delete(body_json);
}

ssize_t person_check_exists(
		const struct PersonRepository* const repo,
		const char* const firstName,
		const char* const lastName,
		const uint8_t age)
{
	for (size_t i = 0; i < repo->n_people; ++i)
	{
		if (strcmp(firstName, repo->people[i].firstName) == 0
			&& strcmp(lastName, repo->people[i].lastName) == 0
			&& age == repo->people[i].age)
			return i;
	}

	return -1;
}

// accept json in below format and add it to repository
/*
 * {
 *     "firstName": "jimmy",
 *     "lastName": "john",
 *     "age": 25
 * }
 */
void person_add(
		struct CallbackArgs* const args)
{
	struct PersonRepository* repo = args->user_data;
	struct Person* person = &repo->people[repo->n_people];

	// parse JSON request body and assign values to struct
	cJSON* request_body_json = cJSON_Parse(args->request_body);
	if (!request_body_json)
	{
		*(args->status_code) = BAD_REQUEST;
		*(args->response_body) = strdup("There was a problem parsing the JSON request.\n");
		return;
	}

	const char* const firstName = cJSON_GetObjectItemCaseSensitive(request_body_json, "firstName")->valuestring;
	const char* const lastName = cJSON_GetObjectItemCaseSensitive(request_body_json, "lastName")->valuestring;
	const uint8_t age = cJSON_GetObjectItemCaseSensitive(request_body_json, "age")->valueint;

	// check if person exists before adding them
	if (person_check_exists(repo, firstName, lastName, age) != -1)
	{
		*(args->status_code) = BAD_REQUEST;
		*(args->response_body) = strdup("That person already exists.\n");
		cJSON_Delete(request_body_json);
		return;
	}
	
	person->firstName = strdup(firstName);
	person->lastName = strdup(lastName);
	person->age = age;

	cJSON_Delete(request_body_json);

	repo->n_people++;

	if (repo->n_people == repo->capacity)
	{
		// dangerously assuming no allocation problems
		repo->capacity *= 2;
		repo->people = realloc(repo->people, repo->capacity * sizeof(struct Person));
		memset(&repo->people[repo->n_people], 0, (repo->capacity - repo->n_people) * sizeof(struct Person));
	}
}

void person_delete(
		struct CallbackArgs* const args)
{
	struct PersonRepository* repo = args->user_data;

	// parse JSON request body and assign values to struct
	cJSON* request_body_json = cJSON_Parse(args->request_body);
	if (!request_body_json)
	{
		*(args->status_code) = BAD_REQUEST;
		*(args->response_body) = strdup("There was a problem parsing the JSON request.\n");
		return;
	}

	const char* const firstName = cJSON_GetObjectItemCaseSensitive(request_body_json, "firstName")->valuestring;
	const char* const lastName = cJSON_GetObjectItemCaseSensitive(request_body_json, "lastName")->valuestring;
	const uint8_t age = cJSON_GetObjectItemCaseSensitive(request_body_json, "age")->valueint;

	ssize_t person_idx = person_check_exists(repo, firstName, lastName, age);
	if (person_idx == -1)
	{
		*(args->status_code) = NOT_FOUND;
		*(args->response_body) = strdup("That person doesn't exist.");
		cJSON_Delete(request_body_json);
		return;
	}

	if (repo->n_people == 0)
		return;

	free(repo->people[person_idx].firstName);
	repo->people[person_idx].firstName = NULL;

	free(repo->people[person_idx].lastName);
	repo->people[person_idx].lastName = NULL;

	for (size_t i = person_idx; i < repo->n_people - 1; ++i)
		memcpy(&repo->people[i], &repo->people[i+1], sizeof(struct Person));
	
	repo->n_people--;

	cJSON_Delete(request_body_json);
}

void person_free(
		void* person_repository)
{
	struct PersonRepository* repo = person_repository;

	// this is a shared pointer, so if it's free'd already,
	// then abort
	if (!repo || !repo->people)
		return;

	for (size_t i = 0; i <= repo->n_people; ++i)
	{
		free(repo->people[i].firstName);
		repo->people[i].firstName = NULL;

		free(repo->people[i].lastName);
		repo->people[i].lastName = NULL;
	}

	free(repo->people);
	repo->people = NULL;
}
