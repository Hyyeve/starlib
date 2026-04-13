#pragma once
#include <filesystem>
#include <format>
#include <functional>
#include <map>
#include <ranges>
#include <string>

#include "status.hpp"

namespace starlib
{
    ///Template function for converting whatever format of resource id you like into resource IDs for the resource manager.
    ///Specialize this function and make your own conversions!
    template <typename value_type>
    std::filesystem::path convert_to_resource_path(const value_type& value)
    {
        return std::filesystem::path(value);
    }

    ///Identifier for a resource via the relative path of the resource.
    struct resource_id
    {
        template <typename value_type>
        // ReSharper disable once CppNonExplicitConvertingConstructor
        resource_id(const value_type& value) : relative_path(convert_to_resource_path(value)) {}

        const std::filesystem::path relative_path;
    };


    struct image_resource
    {
        u8* data; //RGBA ordered, with the number of channels included as specified below. 8-bit channels
        u32 width;
        u32 height;
        u32 num_channels;

        ~image_resource()
        {
            free(data);
        }
    };

    struct hdr_image_resource
    {
        f32* data; //RGBA ordered, with the number of channels included as specified below. 32-bit channels
        u32 width;
        u32 height;
        u32 num_channels;

        ~hdr_image_resource()
        {
            free(data);
        }
    };

    ///Provides functionality for loading and savign resources.
    ///Allows for a 'resource pack' type system via a collection of paths that are searched in order for the asset.
    class resource_manager
    {
    public:

        ///Create a resource manager with an initial root path. The priority of the initial root path is 0.
        ///The default root cannot be removed from the search paths.
        explicit resource_manager(const std::filesystem::path& default_search_path);

        ///Adds a path to the list of paths that will be searched when loading a resource.
        ///Paths with higher priority values will be searched first.
        void add_seach_path(const std::filesystem::path& path, const u32 priority);

        ///Remove a path from the list of search paths.
        ///The default search path cannot be removed.
        void remove_search_path(const std::filesystem::path& path);

        ///Tries to find the resource and outputs the absolute path. False if the resource wasn't found in the search paths.
        bool locate_resource(const resource_id& id, std::filesystem::path& out_path) const;

        ///Loads text from a stream.
        [[nodiscard]] std::string text_from_stream(std::istream& stream, u64 read_pos = 0, u64 size = u64_max) const;
        ///Loads raw bytes from a stream.
        [[nodiscard]] void* bytes_from_stream(std::istream& stream, const u64 read_pos, const u64 size) const;
        ///Loads a image from a stream. The resulting image data will contain null/-1 values if the image load fails.
        [[nodiscard]] image_resource* image_from_stream(std::istream& stream, const u64 read_pos = 0, const u64 size = u64_max) const;
        ///Loads a hdr image from a stream. The resulting image data will contain null/-1 values if the image load fails.
        [[nodiscard]] hdr_image_resource* hdr_image_from_stream(std::istream& stream, const u64 read_pos = 0, const u64 size = u64_max) const;

        ///Tries to create an input stream to read from based on a resource id.
        [[nodiscard]] status create_instream(const resource_id& id, std::ifstream& out_stream, i32 flags = 0) const;
        ///Tries to load a resource into text
        [[nodiscard]] status load_as_text(const resource_id& id, std::string& out_text) const;
        ///Tries to load a resource into raw bytes
        [[nodiscard]] status load_as_bytes(const resource_id& id, void*& out_bytes, u64& out_size) const;

        ///Tries to load a resource into image data
        [[nodiscard]] status load_as_image(const resource_id& id, image_resource*& out_image) const;

        ///Tries to load a resource into hdr image data
        [[nodiscard]] status load_as_hdr_image(const resource_id& id, hdr_image_resource*& out_image) const;

        ///Loads a resource as raw bytes and attempts to convert it into the given struct type.
        template <typename struct_type>
        [[nodiscard]] status load_as_struct(const resource_id id, struct_type& out_struct) const
        {
            void* data;
            u64 size;
            status load_status = load_as_bytes(id, data, size);
            if (load_status.is_error()) return load_status;
            if (size != sizeof(struct_type))
            {
                free(data);
                return {status_type::RANGE_OVERFLOW, std::format("Size of the loaded asset ({0}) does not match the expected size for the struct! ({1})", size, sizeof(struct_type))};
            }

            out_struct = *static_cast<struct_type*>(data);
            free(data);
            return status_type::SUCCESS;
        }

        ///Loads raw bytes from a stream and converts them into the given struct type.
        ///Amount of bytes to read is inferred from the struct size.
        template <typename struct_type>
        [[nodiscard]] struct_type struct_from_stream(std::istream& stream, const u64 start = 0)
        {
            void* data = bytes_from_stream(stream, start, sizeof(struct_type));
            struct_type result = *static_cast<struct_type*>(data);
            free(data);
            return result;
        }

        ///Tries to create an output stream for writing based on a resource id and explicit root path.
        [[nodiscard]] status create_outstream(const resource_id& id, std::ofstream& out_stream, const std::filesystem::path& root, const i32 flags = 0) const;

        ///Tries to create an output stream for writing based on a resource id, relative to the current highest priority root path.
        [[nodiscard]] status create_outstream(const resource_id& id, std::ofstream& out_stream, const i32 flags = 0) const;

        ///Write out raw bytes to a stream
        [[nodiscard]] status write_stream_bytes(std::ostream& stream, const u64 write_pos, const void* data, const u64 size) const;

        ///Write out text to a stream
        [[nodiscard]] status write_stream_text(std::ostream& stream, const u64 write_pos, const std::string_view& text) const;

        ///Write out a png to a stream
        [[nodiscard]] status write_stream_png(std::ostream& stream, const u64 write_pos, const image_resource& image) const;

        ///Write out a jpg to a stream
        [[nodiscard]] status write_stream_jpg(std::ostream& stream, const u64 write_pos, const image_resource& image, const i32 quality) const;

        ///Write out a hdr image to a stream
        [[nodiscard]] status write_stream_hdr(std::ostream& stream, const u64 write_pos, const hdr_image_resource& image) const;

        ///Tries to write out raw bytes to a resource, with an explicit root path.
        [[nodiscard]] status save_as_bytes(const resource_id& id, const void* data, const u64 size, const std::filesystem::path& root) const;

        ///Tries to write out text to a resource, with an explicit root path.
        [[nodiscard]] status save_as_text(const resource_id& id, const std::string_view& text, const std::filesystem::path& root) const;

        ///Tries to write out an image to a png resource, with an explicit root path.
        [[nodiscard]] status save_as_png(const resource_id& id, const image_resource& image, const std::filesystem::path& root) const;

        ///Tries to write out an image to a png resource, with an explicit root path.
        [[nodiscard]] status save_as_jpg(const resource_id& id, const image_resource& image, const std::filesystem::path& root, const i32 quality) const;

        ///Tries to write out an image to a png resource, with an explicit root path.
        [[nodiscard]] status save_as_hdr(const resource_id& id, const hdr_image_resource& image, const std::filesystem::path& root) const;

        ///Tries to write out raw bytes to a resource, relative to the current highest priority root path.
        [[nodiscard]] status save_as_bytes(const resource_id& id, const void* data, const u64 size) const;

        ///Tries to write out text to a resource, relative to the current highest priority root path.
        [[nodiscard]] status save_as_text(const resource_id& id, const std::string_view& text) const;

        ///Tries to write out an image to a resource, relative to the current highest priority root path.
        [[nodiscard]] status save_as_png(const resource_id& id, const image_resource& image_data) const;

        ///Tries to write out an image to a resource, relative to the current highest priority root path.
        [[nodiscard]] status save_as_jpg(const resource_id& id, const image_resource& image_data, const i32 quality) const;

        ///Tries to write out an image to a resource, relative to the current highest priority root path.
        [[nodiscard]] status save_as_hdr(const resource_id& id, const hdr_image_resource& image_data) const;

    private:
        std::multimap<u32, std::filesystem::path> roots;
        std::filesystem::path default_root;
    };
}
