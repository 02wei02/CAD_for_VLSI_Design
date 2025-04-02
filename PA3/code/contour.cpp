/* Find the contour path of y-axis */
#include "contour.h"
#include "structure.h"

using namespace std;

void Tour::insert(double x1, double x2, double y, Contour *&head)
{
    Contour *new_Contour = new Contour();

    new_Contour->x1 = x1;
    new_Contour->x2 = x2;
    new_Contour->y = y;
    // it there is no root in contour add to the head
    if (head == nullptr || head->x1 >= x2)
    {
        new_Contour->next = head;
        head = new_Contour;
        return;
    }

    // find the correct location in order
    Contour *current = head;
    while (current->next != nullptr && current->next->x1 < x1)
    {
        current = current->next;
    }

    new_Contour->next = current->next;
    current->next = new_Contour;
}

void Tour::clear(Contour *&head)
{
    Contour *current = head;
    while (current != nullptr)
    {
        Contour *next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
}

// delete the match node
void Tour::remove(double x1, double x2, double y, Contour *&head)
{
    Contour *current = head;
    Contour *previous = nullptr;

    while (current != nullptr && (current->x1 != x1 || current->x2 != x2 || current->y != y))
    {
        previous = current;
        current = current->next;
    }

    if (current == nullptr)
        return;

    if (previous == nullptr)
    {
        head = current->next;
    }
    else
    {
        previous->next = current->next;
    }

    delete current;
}

// refresh the contour，adjust its scale
void Tour::refresh(double x1, double x2, double y, Contour *&head)
{
    Contour *current = head;
    Contour *previous = nullptr;
    if (head == nullptr)
        insert(x1, x2, y, head);
    else
    {
        // adjust the node in neighborhood
        while (current != nullptr && current->x1 < x2)
        {
            if (y >= current->y)
            {
                if (current->x1 >= x1 && current->x1 <= x2) // x1 <= current->x1 <= x2
                {
                    current->x1 = x2;
                }
                if (current->x2 >= x1 && current->x2 <= x2) // x1 <= current->x2 <= x2
                {
                    current->x2 = x1;
                }
                if (current->x1 >= x1 && current->x2 <= x2) // x1 <= current->x1 < current -> x2 <= x2
                {
                    Contour *temp = current->next;
                    remove(current->x1, current->x2, current->y, head);
                    current = temp;
                    continue;
                }
            }
            previous = current;
            current = current->next;
        }
        // insert new update node
        insert(x1, x2, y, head);
    }
}

void Tour::printContour(Contour *head)
{
    Contour *current = head;
    while (current != nullptr)
    {
        cout << "(" << current->x1 << ", " << current->x2 << ", " << current->y << ") -> ";
        current = current->next;
    }
    cout << "nullptr" << endl;
}
double Tour::findY(double x1, double x2, Contour *head)
{
    double max = 0;
    x1 = x1 + 0.001;
    if (head == nullptr)
    {
        return max;
    }
    Contour *current = head;

    while (current != nullptr)
    {
        // Skip nodes that are completely to the left of the interval [x1, x2]
        while (current != nullptr && current->x2 <= x1) // if the x1 = x2, skip this node. We don't count in bound
        {
            current = current->next;
        }
        // Process nodes that intersect with the interval [x1, x2]
        while (current != nullptr && current->x1 <= x2)
        {
            if (current->x2 == x1)
                break;
            if (max < current->y)
            {
                max = current->y;
            }
            current = current->next;
        }
        // Break out of the outer while loop if current is nullptr
        return max;
    }
    cout << "max y 3: " << max << endl;
    return max;
}