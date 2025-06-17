#pragma once
#include "elements.h"
#include <vector>

class BubbleSorter
{
  public:
    BubbleSorter(std::vector<SortingPillar> &pillars) : m_pillars(pillars) { reset(); }
    void reset()
    {
        m_i = 0;
        m_j = 0;
        m_swapped = false;
        m_done = false;
    }
    void step()
    {
        if (m_done)
        {
            for (auto &it : m_pillars)
                it.setFocus(true);
            return;
        }
        int n = m_pillars.size();
        m_pillars[m_j].setFocus(false);
        m_pillars[m_j + 1].setFocus(false);
        if (m_pillars[m_j].m_value > m_pillars[m_j + 1].m_value)
        {
            std::swap(m_pillars[m_j].m_value, m_pillars[m_j + 1].m_value);
            std::swap(m_pillars[m_j].m_position, m_pillars[m_j + 1].m_position);
            m_swapped = true;
        }
        m_j++;
        if (m_j >= n - m_i - 1)
        {
            if (!m_swapped)
            {
                m_done = true;
                for (auto &it : m_pillars)
                    it.setFocus(true);
                return;
            }
            m_swapped = false;
            m_j = 0;
            m_i++;
            if (m_i >= n - 1)
            {
                m_done = true;
                for (auto &it : m_pillars)
                    it.setFocus(true);
            }
        }
        m_pillars[m_j].setFocus(true);
        m_pillars[m_j + 1].setFocus(true);
    }
    bool isDone() { return m_done; }

  private:
    std::vector<SortingPillar> &m_pillars;
    int m_i, m_j;
    bool m_swapped;
    bool m_done;
};

class SelectionSorter
{
  public:
    SelectionSorter(std::vector<SortingPillar> &pillars) : m_pillars(pillars) { reset(); }

    void reset()
    {
        m_i = 0;
        m_j = 1;
        m_minIndex = 0;
        m_done = false;
        for (auto &p : m_pillars)
            p.setFocus(false);
    }

    void step()
    {
        int n = m_pillars.size();
        if (m_done)
        {
            for (auto &p : m_pillars)
                p.setFocus(true);
            return;
        }

        // clear previous highlights
        for (auto &p : m_pillars)
            p.setFocus(false);

        // scan for new minimum in [m_i+1 .. n)
        if (m_pillars[m_j].m_value < m_pillars[m_minIndex].m_value) m_minIndex = m_j;

        // advance j
        ++m_j;
        if (m_j >= n)
        {
            // end of scan: swap min into position m_i
            if (m_minIndex != m_i)
            {
                std::swap(m_pillars[m_i].m_value, m_pillars[m_minIndex].m_value);
                std::swap(m_pillars[m_i].m_position, m_pillars[m_minIndex].m_position);
            }
            ++m_i;
            if (m_i >= n - 1)
            {
                m_done = true;
                for (auto &p : m_pillars)
                    p.setFocus(true);
                return;
            }
            // reset scan for next pass
            m_minIndex = m_i;
            m_j = m_i + 1;
        }

        // highlight boundary and current minimum
        m_pillars[m_i].setFocus(true);
        m_pillars[m_j].setFocus(true);
        m_pillars[m_minIndex].setFocus(true);
    }

    bool isDone() const { return m_done; }

  private:
    std::vector<SortingPillar> &m_pillars;
    int m_i, m_j, m_minIndex;
    bool m_done;
};

class InsertionSorter
{
  public:
    InsertionSorter(std::vector<SortingPillar> &pillars) : m_pillars(pillars) { reset(); }

    void reset()
    {
        m_i = 1; // next element to “insert”
        m_j = 1; // scan position
        m_done = false;
        for (auto &p : m_pillars)
            p.setFocus(false);
    }

    void step()
    {
        int n = m_pillars.size();
        if (m_done)
        {
            for (auto &p : m_pillars)
                p.setFocus(true);
            return;
        }

        // clear last highlight
        if (m_j > 0)
        {
            m_pillars[m_j].setFocus(false);
            m_pillars[m_j - 1].setFocus(false);
        }

        // if out of bounds or in right place, advance i
        if (m_j == 0 || m_pillars[m_j].m_value >= m_pillars[m_j - 1].m_value)
        {
            m_i++;
            m_j = m_i;
            if (m_i >= n)
            {
                m_done = true;
                for (auto &p : m_pillars)
                    p.setFocus(true);
                return;
            }
        }
        // otherwise, swap down one step
        else
        {
            std::swap(m_pillars[m_j].m_value, m_pillars[m_j - 1].m_value);
            std::swap(m_pillars[m_j].m_position, m_pillars[m_j - 1].m_position);
            m_j--;
        }

        // highlight the two being compared/swapped
        if (m_j > 0)
        {
            m_pillars[m_j].setFocus(true);
            m_pillars[m_j - 1].setFocus(true);
        }
    }

    bool isDone() const { return m_done; }

  private:
    std::vector<SortingPillar> &m_pillars;
    int m_i, m_j;
    bool m_done;
};