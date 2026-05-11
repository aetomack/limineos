#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>

/*
 * Set base revision to most updated limine version, 6.
 * See spec: https://github.com/Limine-Bootloader/limine-protocol/blob/trunk/PROTOCOL.md#base-revision-6
 */

__attribute__((used, section(".limine_requests")))
static volatile uint64_t base_revision[] = LIMINE_BASE_REVISION(6);

/*
* Limine Requests can be placed anywhere, but it is important that the compiler doesn't optimize that way.
* Therefore, it's important they're markedd volatile, AND they should be accessed at least once or marked as used 
* with the "used" attribute as done here.
*/
