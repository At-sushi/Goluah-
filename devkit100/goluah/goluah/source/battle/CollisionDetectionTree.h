/*============================================================================

    Goluah!! Copyright (C) 2001-2004 aki, 2014-2015 logger, 2004-2020 At-sushi

    This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

==============================================================================*/

#pragma once
#include <deque>
#include <cassert>
#include <functional>

// �����蔻��p4�����Z�O�����g�c���[
// �L�������P�[�X�ƃX�l�[�N�P�[�X����������ɂȂ��Ă�
template<class T, class ContainerHeap = typename std::deque<T>> class CollisionDetectionTree
{
public:
    CollisionDetectionTree()
    {
    }

    // �Z�O�����g�c���[�ɃI�u�W�F�N�g��}��
    void insert(int left, int top, int right, int bottom, const T& obj) {
        assert((left <= right) && (top <= bottom));

        int node_current = 0, x1 = X_MIN, y1 = Y_MIN, x2 = X_MAX, y2 = Y_MAX;

        for (;;) {
            assert((x1 < x2) && (y1 < y2));
            const int x_mid = (x2 - x1) / 2 + x1, y_mid = (y2 - y1) / 2 + y1;

            // ���E�����܂����ł��邩����
            if ((getFirstChildNode(node_current) >= SIZE_HEAP) ||
                (left <= x_mid && x_mid <= right) ||
                (top <= y_mid && y_mid <= bottom)) {
                segmentHeap[node_current].origin.push_back(obj);
                break;
            }
            else {
                segmentHeap[node_current].sub.push_back(obj);
                node_current = getFirstChildNode(node_current);

                // Z�^��4��������
                if (x_mid < left) {
                    node_current += 1;
                    x1 = x_mid;
                }
                else
                    x2 = x_mid;

                if (y_mid < top) {
                    node_current += 2;
                    y1 = y_mid;
                }
                else
                    y2 = y_mid;

                assert(node_current < SIZE_HEAP);
            }
        }
    }

    // �S����
    void clear() {
        for (auto&& i : segmentHeap) {
            i.origin.clear();
            i.sub.clear();
        }
    }

    // �S�I�u�W�F�N�g���m�̓����蔻������s
    template<typename F> void collideAll(F func_collide) {
        std::function<void(int)> searchHeap = [&](int node_current) {
            assert(node_current < SIZE_HEAP);

            // �m�[�h�̏Փ˔���
            // ��������Ă�̂Őe�m�[�h�ɑ΂��Ă͔��肵�Ȃ�
            for (auto it = segmentHeap[node_current].origin.cbegin(); it != segmentHeap[node_current].origin.cend(); ++it) {
                // ���̓��K�w�̃I�u�W�F�N�g�ɑ΂���
                for (auto ite = it + 1; ite != segmentHeap[node_current].origin.cend(); ++ite)
                    func_collide(*it, *ite);

                // �q�m�[�h�̃I�u�W�F�N�g�ɑ΂���
                for (const auto& j : segmentHeap[node_current].sub)
                    func_collide(*it, j);
            }

            // ���ɏՓ˂������ȃI�u�W�F�N�g������ꍇ
            if (segmentHeap[node_current].sub.size() >= 2) {
                const int node_next = getFirstChildNode(node_current);
                // �q�m�[�h��T��
                searchHeap(node_next);
                searchHeap(node_next + 1);
                searchHeap(node_next + 2);
                searchHeap(node_next + 3);
            }
        };

        // �q�[�v�S�̂�T��
        searchHeap(0);
    }

    template<typename F> void collideWithAABB(int left, int top, int right, int bottom, F func_collide) {
        assert((left <= right) && (top <= bottom));
        std::function<void(int,int,int,int,int)> searchHeap = [&](int node_current, int x1, int y1, int x2, int y2) {
            assert((x1 < x2) && (y1 < y2));
            assert(node_current < SIZE_HEAP);
            const int node_next = getFirstChildNode(node_current);
            if ((node_next >= SIZE_HEAP) || ((left <= x1) && (top <= y1) && (right >= x2) && (bottom >= y2))) {
                // AABB����Ԃ����S�Ɋ܂�ł���ꍇ
                for (const auto& i : segmentHeap[node_current].origin)
                    func_collide(i);
                for (const auto& i : segmentHeap[node_current].sub)
                    func_collide(i);
            }
            else if ((left <= x2) && (top <= y2) && (right >= x1) && (bottom >= y1)) {
                // AABB����Ԃ̈ꕔ���܂�ł���ꍇ
                const int x_mid = (x2 - x1) / 2 + x1, y_mid = (y2 - y1) / 2 + y1;

                for (const auto& i : segmentHeap[node_current].origin)
                    func_collide(i);

                // �q�m�[�h��T��
                searchHeap(node_next, x1, y1, x_mid, y_mid);
                searchHeap(node_next + 1, x_mid, y1, x2, y_mid);
                searchHeap(node_next + 2, x1, y_mid, x_mid, y2);
                searchHeap(node_next + 3, x_mid, y_mid, x2, y2);
            }
        };

        searchHeap(0, X_MIN, Y_MIN, X_MAX, Y_MAX);
    }

private:
    enum {
        SEGMENT_DEPTH = 4,										//! 4���؂̊K�w��
        SIZE_HEAP = ((1 << SEGMENT_DEPTH * 2) - 1) / (4 - 1),	//! �q�[�v�̃T�C�Y
        X_MIN = -640, X_MAX = 640,
        Y_MIN = X_MIN, Y_MAX = X_MAX
    };
    static_assert(X_MIN < X_MAX, "object range is invalid");
    static_assert(Y_MIN < Y_MAX, "object range is invalid");

    // �q�m�[�h�̍ŏ���index���擾
    static int getFirstChildNode(int index) { return index * 4 + 1; }

    struct {
        // origin: ���C���[���̃I�u�W�F�N�g�̃��X�g�Asub: ���̕��ɂ���I�u�W�F�N�g�̃��X�g
        ContainerHeap origin, sub;
    } segmentHeap[SIZE_HEAP];
};

