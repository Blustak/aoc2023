const std = @import("std");
const expect = @import("std").testing.expect;

//A few helpful things compiled away at compile time
const PATH = "input.txt";

pub fn main() !void {
    const file = try std.fs.cwd().openFile(PATH, .{});
    defer file.close();

    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const ally = arena.allocator();

    const read_buf: [:0]const u8 = try file.readToEndAllocOptions(ally, 1024 * 1024, null, 1, 0);
    defer ally.free(read_buf);

    const sol = solution(read_buf);

    std.debug.print("The solution I found was {d}", .{sol});
}

fn solution(s_ptr: [:0]const u8) u32 {
    var sum: u32 = 0;
    var first: ?u8 = null;
    var last: ?u8 = null;
    var c: u8 = s_ptr[0];
    var i: u32 = 0;

    while (true) : ({
        c = s_ptr[i];
        i += 1;
    }) {
        switch (c) {
            49...57 => {
                first = first orelse (c - '0');
                last = c - '0';
            },

            '\n', 0 => {
                sum += (@as(u32, first orelse 0) * 10) + @as(u32, last orelse 0); //Deals with empty lines
                first = null;
                last = null;
                if (c == 0) {
                    break;
                }
            },
            else => continue,
        }
    }
    return sum;
}

test "solution test" {
    const test_input: [:0]const u8 =
        \\1abc2
        \\pqr3stu8vwx
        \\a1b2c3d4e5f
        \\treb7uchet
    ;

    const output = solution(test_input);
    try expect(output == 142);
}
