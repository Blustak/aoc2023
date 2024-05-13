const std = @import("std");
const input = @embedFile("input.txt");

const line_len = blk: {
    var i: u32 = 0;
    while (i < input.len and input[i] != '\n') : (i += 1) {}
    break :blk i;
};

const line_no = @as(u32, @divFloor(input.len, line_len)) - 1;

const Nav = struct {
    up: bool,
    left: bool,
    down: bool,
    right: bool,

    pub fn new(x_pos: u32, y_pos: u32, len: u32, x_max: u32, y_max: u32) Nav {
        return Nav{
            .up = y_pos > 0,
            .left = x_pos > 0,
            .down = y_pos < y_max - 1,
            .right = x_pos + len < x_max,
        };
    }
};

pub fn main() !void {
    std.debug.print("My solution to day 3 is:{!}\n", .{solution(input, line_len, line_no)});
}

fn solution(s_in: []const u8, l_len: comptime_int, l_no: comptime_int) !u32 {
    var sum: u32 = 0;

    const matrix = init: {
        var initial_val: [l_no][l_len]u8 = undefined;
        var lines = std.mem.tokenizeScalar(u8, s_in, '\n');
        for (&initial_val) |*pt| {
            std.mem.copyForwards(u8, &pt.*, lines.next() orelse undefined);
        }
        break :init initial_val;
    };
    var y: u32 = 0;

    while (y < l_no) : (y += 1) {
        var x: u32 = 0;
        no_capture: while (x < l_len) : (x += 1) {
            switch (matrix[y][x]) {
                '0'...'9' => {
                    const x_start: u32 = x;
                    var no: u32 = 0;
                    var len: u8 = 0; // it's already of length 1 at least.

                    while (x < l_len and std.ascii.isDigit(matrix[y][x])) {
                        no = (no * 10) + matrix[y][x] - '0';
                        x += 1;
                        len += 1;
                    }

                    const nav = Nav.new(x_start, y, len, l_len, l_no);

                    if (nav.up) {
                        for (matrix[y - 1][x_start..x]) |c| {
                            if (c != '.') {
                                sum += no;
                                //std.debug.print("Found {d} at x:{d}-{d}, y:{d}. Sum is {d}\n", .{ no, x_start, x, y, sum });
                                continue :no_capture;
                            }
                        }
                    }
                    if (nav.down) {
                        for (matrix[y + 1][x_start..x]) |c| {
                            if (c != '.') {
                                sum += no;
                                //std.debug.print("Found {d} at x:{d}-{d}, y:{d}. Sum is {d}\n", .{ no, x_start, x, y, sum });
                                continue :no_capture;
                            }
                        }
                    }
                    if (nav.left) {
                        for ([_]u8{ if (nav.up) matrix[y - 1][x_start - 1] else '.', matrix[y][x_start - 1], if (nav.down) matrix[y + 1][x_start - 1] else '.' }) |c| {
                            if (c != '.') {
                                sum += no;
                                //std.debug.print("Found {d} at x:{d}-{d}, y:{d}. Sum is {d}\n", .{ no, x_start, x, y, sum });
                                continue :no_capture;
                            }
                        }
                    }
                    if (nav.right) {
                        for ([_]u8{ if (nav.up) matrix[y - 1][x] else '.', matrix[y][x], if (nav.down) matrix[y + 1][x] else '.' }) |c| {
                            if (c != '.') {
                                sum += no;
                                //std.debug.print("Found {d} at x:{d}-{d}, y:{d}. Sum is {d}\n", .{ no, x_start, x, y, sum });
                                continue :no_capture;
                            }
                        }
                    }
                },
                else => {},
            }
        }
    }
    return sum;
}

test "sample code" {
    const test_input =
        \\467..114..
        \\...*......
        \\..35..633.
        \\......#...
        \\617*......
        \\.....+.58.
        \\..592.....
        \\......755.
        \\...$.*....
        \\.664.598..
    ;

    const l_len = t1: {
        comptime var i: u32 = 0;
        inline while (i < test_input.len and test_input[i] != '\n') : (i += 1) {}
        break :t1 i;
    };
    const l_no = t2: {
        comptime var count: u32 = 1; //Lets count the end of file line here
        inline for (test_input) |c| {
            if (c == '\n') count += 1;
        }
        break :t2 count;
    };

    const result = try solution(test_input, l_len, l_no);
    std.debug.print("\ntest output:{!}\n", .{result});

    try std.testing.expect(result == 4361);
}
