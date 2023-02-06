#ifndef PERSON_H
#define PERSON_H

#include <stddef.h>
#include <stdint.h>

// forward declaration
struct CallbackArgs;

struct Person
{
	char* firstName;
	char* lastName;
	uint8_t age;
};

struct PersonRepository
{
	struct Person* people;
	size_t n_people;
	size_t capacity;
};

void person_init(
		struct PersonRepository* const people,
		size_t capacity);

// GET callback for /api/v1/person/getAll
void person_get_all(
		struct CallbackArgs* const args);

// POST callback for /api/v1/person/add
void person_add(
		struct CallbackArgs* const args);

// DELETE callback for /api/v1/person/delete
void person_delete(
		struct CallbackArgs* const args);

// deallocator (note, it requires a void* because
// that's how cerveurus expects the callback to look like)
void person_free(
		void* person_repository);

		

#endif
