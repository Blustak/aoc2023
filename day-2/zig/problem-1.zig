const std = @import("std");

const PATH = "input.txt";

const MAX_RED = 12;
const MAX_GREEN = 13;
const MAX_BLUE = 14;

pub fn main() !void {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();

    const ally = arena.allocator();

    const buf = try std.fs.Dir.readFileAlloc(std.fs.cwd(), ally, PATH, 1024 * 1024);
    defer ally.free(buf);

    _ = solution(&buf);
}

fn solution(input: *const []const u8) u32 {
    var sum: u32 = 0;
    var id: u8 = 1; //Hack to ignore reading the id.
    var iter = std.mem.splitScalar(u8, input.*, '\n');

    round_loop: while (iter.next()) |line| : (id += 1) {
        if (line.len == 0) {
            continue;
        }
        var working_iter = std.mem.splitScalar(u8, line[std.mem.indexOfScalar(u8, line, ':') orelse 0 ..], ';');

        while (working_iter.next()) |round| {
            var quantity: u8 = 0;
            var found_col: bool = false;
            for (round) |c| {
                if (!found_col) {
                    switch (c) {
                        '0'...'9' => {
                            quantity = (quantity * 10) + (c - '0');
                        },
                        'r', 'g', 'b' => |byte| {
                            const max_quant: u8 = switch (byte) {
                                'r' => MAX_RED,
                                'g' => MAX_GREEN,
                                'b' => MAX_BLUE,
                                else => unreachable,
                            };
                            if (quantity > max_quant) {
                                continue :round_loop; //Move to the next line.
                            }
                            found_col = true;
                        },
                        else => {},
                    }
                } else if (c == ',') {
                    quantity = 0;
                    found_col = false;
                }
            }
        }
        sum += id;
        std.debug.print("Game {d} is legal! Sum is now {d}.\n", .{ id, sum });
    }

    std.debug.print("\n", .{}); //Stops zig tests eating the output.

    return sum;
}

test "sample test" {
    const input: []const u8 =
        \\Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
        \\Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue
        \\Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
        \\Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red
        \\Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green
    ;

    std.debug.print("\n", .{});

    try std.testing.expect(solution(&input) == 8);
}
