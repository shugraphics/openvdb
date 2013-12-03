///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2012-2013 DreamWorks Animation LLC
//
// All rights reserved. This software is distributed under the
// Mozilla Public License 2.0 ( http://www.mozilla.org/MPL/2.0/ )
//
// Redistributions of source code must retain the above copyright
// and license notice and the following restrictions and disclaimer.
//
// *     Neither the name of DreamWorks Animation nor the names of
// its contributors may be used to endorse or promote products derived
// from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// IN NO EVENT SHALL THE COPYRIGHT HOLDERS' AND CONTRIBUTORS' AGGREGATE
// LIABILITY FOR ALL CLAIMS REGARDLESS OF THEIR BASIS EXCEED US$250.00.
//
///////////////////////////////////////////////////////////////////////////

#include <openvdb/metadata/MetaMap.h>
#include <openvdb/util/logging.h>
#include <sstream>


namespace openvdb {
OPENVDB_USE_VERSION_NAMESPACE
namespace OPENVDB_VERSION_NAME {

MetaMap::MetaMap(const MetaMap &other)
{
    // Insert all metadata into this map.
    ConstMetaIterator iter = other.beginMeta();
    for( ; iter != other.endMeta(); ++iter) {
        this->insertMeta(iter->first, *(iter->second));
    }
}


MetaMap::Ptr
MetaMap::copyMeta() const
{
    MetaMap::Ptr ret(new MetaMap);
    ret->mMeta = this->mMeta;
    return ret;
}


MetaMap::Ptr
MetaMap::deepCopyMeta() const
{
    return MetaMap::Ptr(new MetaMap(*this));
}


MetaMap&
MetaMap::operator=(const MetaMap& other)
{
    if (&other != this) {
        this->clearMetadata();
        // Insert all metadata into this map.
        ConstMetaIterator iter = other.beginMeta();
        for ( ; iter != other.endMeta(); ++iter) {
            this->insertMeta(iter->first, *(iter->second));
        }
    }
    return *this;
}


void
MetaMap::readMeta(std::istream &is)
{
    // Clear out the current metamap if need be.
    this->clearMetadata();

    // Read in the number of metadata items.
    Index32 count = 0;
    is.read(reinterpret_cast<char*>(&count), sizeof(Index32));

    // Read in each metadata.
    for (Index32 i = 0; i < count; ++i) {
        // Read in the name.
        Name name = readString(is);

        // Read in the metadata typename.
        Name typeName = readString(is);

        // Create a metadata type from the typename. Make sure that the type is
        // registered.
        if (!Metadata::isRegisteredType(typeName)) {
            OPENVDB_LOG_WARN("cannot read metadata \"" << name
                << "\" of unregistered type \"" << typeName << "\"");
            UnknownMetadata metadata;
            metadata.read(is);
        } else {
            Metadata::Ptr metadata = Metadata::createMetadata(typeName);

            // Read the value from the stream.
            metadata->read(is);

            // Add the name and metadata to the map.
            insertMeta(name, *metadata);
        }
    }
}

void
MetaMap::writeMeta(std::ostream &os) const
{
    // Write out the number of metadata items we have in the map. Note that we
    // save as Index32 to save a 32-bit number. Using size_t would be platform
    // dependent.
    Index32 count = (Index32)metaCount();
    os.write(reinterpret_cast<char*>(&count), sizeof(Index32));

    // Iterate through each metadata and write it out.
    for (ConstMetaIterator iter = beginMeta(); iter != endMeta(); ++iter) {
        // Write the name of the metadata.
        writeString(os, iter->first);

        // Write the type name of the metadata.
        writeString(os, iter->second->typeName());

        // Write out the metadata value.
        iter->second->write(os);
    }
}

void
MetaMap::insertMeta(const Name &name, const Metadata &m)
{
    if(name.size() == 0)
        OPENVDB_THROW(ValueError, "Metadata name cannot be an empty string");

    // See if the value already exists, if so then replace the existing one.
    MetaIterator iter = mMeta.find(name);

    if(iter == mMeta.end()) {
        // Create a copy of hte metadata and store it in the map
        Metadata::Ptr tmp = m.copy();
        mMeta[name] = tmp;
    } else {
        if(iter->second->typeName() != m.typeName()) {
            std::ostringstream ostr;
            ostr << "Cannot assign value of type "
                 << m.typeName() << " to metadata attribute " << name
                 << " of " << "type " << iter->second->typeName();
            OPENVDB_THROW(TypeError, ostr.str());
        }
        // else
        Metadata::Ptr tmp = m.copy();
        iter->second = tmp;
    }
}

void
MetaMap::removeMeta(const Name &name)
{
    // Find the required metadata
    MetaIterator iter = mMeta.find(name);

    if(iter == mMeta.end())
        return;
    // else, delete the metadata and remove from the map
    mMeta.erase(iter);
}


std::string
MetaMap::str() const
{
    std::ostringstream buffer;
    buffer << "MetaMap:\n";
    for(ConstMetaIterator iter = beginMeta(); iter != endMeta(); ++iter) {
        buffer << "    " << iter->first << " = " << iter->second->str();
        buffer << std::endl;
    }
    return buffer.str();
}

std::ostream&
operator<<(std::ostream& ostr, const MetaMap& metamap)
{
    ostr << metamap.str();
    return ostr;
}

} // namespace OPENVDB_VERSION_NAME
} // namespace openvdb

// Copyright (c) 2012-2013 DreamWorks Animation LLC
// All rights reserved. This software is distributed under the
// Mozilla Public License 2.0 ( http://www.mozilla.org/MPL/2.0/ )
