// inheritance.c
#define _DEFAULT_SOURCE
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Each person has two parents and two alleles
typedef struct person
{
    struct person *parents[2];
    char alleles[2];
} person;

const int GENERATIONS = 3;
const int INDENT_LENGTH = 4;

person *create_family(int generations);
void print_family(person *p, int generation);
void free_family(person *p);
char random_allele();

int main(void)
{
    // Seed random number generator
    srandom(time(0));

    // Create a new family with three generations
    person *p = create_family(GENERATIONS);

    // Print family tree of blood types
    print_family(p, 0);

    // Free memory
    free_family(p);
}

// Create a new individual with `generations`
person *create_family(int generations)
{
    // Allocate memory for new person
    person *new_person = malloc(sizeof(person));
    if (new_person == NULL)
    {
        fprintf(stderr, "Could not allocate memory for new person.\n");
        exit(1);
    }

    // If there are still generations left to create
    if (generations > 1)
    {
        // Recursively create parents
        person *parent0 = create_family(generations - 1);
        person *parent1 = create_family(generations - 1);

        // Set parent pointers
        new_person->parents[0] = parent0;
        new_person->parents[1] = parent1;

        // Inherit one random allele from each parent
        new_person->alleles[0] = parent0->alleles[random() % 2];
        new_person->alleles[1] = parent1->alleles[random() % 2];
    }
    else
    {
        // Oldest generation has no parents
        new_person->parents[0] = NULL;
        new_person->parents[1] = NULL;

        // Randomly assign two alleles
        new_person->alleles[0] = random_allele();
        new_person->alleles[1] = random_allele();
    }

    return new_person;
}

// Free `p` and all ancestors of `p`.
void free_family(person *p)
{
    // Base case: nothing to free
    if (p == NULL)
    {
        return;
    }

    // Free parents recursively
    free_family(p->parents[0]);
    free_family(p->parents[1]);

    // Free this person
    free(p);
}

// Print each family member and their alleles.
void print_family(person *p, int generation)
{
    // Base case
    if (p == NULL)
    {
        return;
    }

    // Print indentation
    for (int i = 0; i < generation * INDENT_LENGTH; i++)
    {
        printf(" ");
    }

    // Label based on generation
    if (generation == 0)
    {
        printf("Child (Generation %i): blood type %c%c\n",
               generation, p->alleles[0], p->alleles[1]);
    }
    else if (generation == 1)
    {
        printf("Parent (Generation %i): blood type %c%c\n",
               generation, p->alleles[0], p->alleles[1]);
    }
    else
    {
        for (int i = 0; i < generation - 2; i++)
        {
            printf("Great-");
        }
        printf("Grandparent (Generation %i): blood type %c%c\n",
               generation, p->alleles[0], p->alleles[1]);
    }

    // Recurse on parents
    print_family(p->parents[0], generation + 1);
    print_family(p->parents[1], generation + 1);
}

// Randomly chooses a blood type allele.
char random_allele()
{
    int r = random() % 3;
    if (r == 0)
    {
        return 'A';
    }
    else if (r == 1)
    {
        return 'B';
    }
    else
    {
        return 'O';
    }
}
